#version 400
layout(location=0) out vec4 FragColor;
uniform mat4 ogre_ModelViewProjectMatrix;
uniform sampler2D postexture;
uniform sampler2D normtexture;
uniform sampler3D offsettexture;
uniform vec3 lightDir;
uniform float shininess;
uniform vec3 OffsetTexSize;
uniform float aoDensity;
uniform float contrasty;
uniform int useSSAO;
uniform float farDis;
in vec2 tcoord;

void makeRotateToQuad( in vec3 from, in vec3 to, out vec4 quad )
{
	// This routine takes any vector as argument but normalized
	// vectors are necessary, if only for computing the dot product.
	// Too bad the API is that generic, it leads to performance loss.
	// Even in the case the 2 vectors are not normalized but same length,
	// the sqrt could be shared, but we have no way to know beforehand
	// at this point, while the caller may know.
	// So, we have to test... in the hope of saving at least a sqrt
	vec3 sourceVector = from;
	vec3 targetVector = to;

	float fromLen2 = from.x * from.x + from.y * from.y + from.z * from.z;
	float fromLen;
	// normalize only when necessary, epsilon test
	if ((fromLen2 < 1.0-1e-7) || (fromLen2 > 1.0+1e-7)) {
		fromLen = sqrt(fromLen2);
		sourceVector /= fromLen;
	} else fromLen = 1.0;

	float toLen2 = to.x * to.x + to.y * to.y + to.z * to.z;
	// normalize only when necessary, epsilon test
	if ((toLen2 < 1.0-1e-7) || (toLen2 > 1.0+1e-7)) {
		float toLen;
		// re-use fromLen for case of mapping 2 vectors of the same length
		if ((toLen2 > fromLen2-1e-7) && (toLen2 < fromLen2+1e-7)) {
			toLen = fromLen;
		}
		else toLen = sqrt(toLen2);
		targetVector /= toLen;
	}


	// Now let's get into the real stuff
	// Use "dot product plus one" as test as it can be re-used later on
	float dotProdPlus1 = 1.0 + dot(sourceVector, targetVector);

	// Check for degenerate case of full u-turn. Use epsilon for detection
	if (dotProdPlus1 < 1e-7) {

		// Get an orthogonal vector of the given vector
		// in a plane with maximum vector coordinates.
		// Then use it as quaternion axis with pi angle
		// Trick is to realize one value at least is >0.6 for a normalized vector.
		if (abs(sourceVector.x) < 0.6) {
			float norm = sqrt(1.0 - sourceVector.x * sourceVector.x);
			quad.x = 0.0;
			quad.y = sourceVector.z / norm;
			quad.z = -sourceVector.y / norm;
			quad.w = 0.0;
		} else if (abs(sourceVector.y) < 0.6) {
			float norm = sqrt(1.0 - sourceVector.y * sourceVector.y);
			quad.x = -sourceVector.z / norm;
			quad.y = 0.0;
			quad.z = sourceVector.x / norm;
			quad.w = 0.0;
		} else {
			float norm = sqrt(1.0 - sourceVector.z * sourceVector.z);
			quad.x = sourceVector.y / norm;
			quad.y = -sourceVector.x / norm;
			quad.z = 0.0;
			quad.w = 0.0;
		}
	}

	else {
		// Find the shortest angle quaternion that transforms normalized vectors
		// into one other. Formula is still valid when vectors are colinear
		float s = sqrt(0.5 * dotProdPlus1);
		vec3 tmp = cross(sourceVector, targetVector / (2.0*s));
		quad.x = tmp.x;
		quad.y = tmp.y;
		quad.z = tmp.z;
		quad.w = s;
	}
}

void getRotateMat( in vec4 rot, out mat4 m )
{
	      float length2 = rot.x * rot.x + rot.y * rot.y + rot.z * rot.z + rot.w * rot.w;
	      float rlength2;
        // normalize quat if required.
        // We can avoid the expensive sqrt in this case since all 'coefficients' below are products of two q components.
        // That is a square of a square root, so it is possible to avoid that
        if (length2 != 1.0)
        {
            rlength2 = 2.0/length2;
        }
        else
        {
            rlength2 = 2.0;
        }

        // Source: Gamasutra, Rotating Objects Using Quaternions
        //
        //http://www.gamasutra.com/features/19980703/quaternions_01.htm

        float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

        // calculate coefficients
        x2 = rlength2*rot.x;
        y2 = rlength2*rot.y;
        z2 = rlength2*rot.z;

        xx = rot.x * x2;
        xy = rot.x * y2;
        xz = rot.x * z2;

        yy = rot.y * y2;
        yz = rot.y * z2;
        zz = rot.z * z2;

        wx = rot.w * x2;
        wy = rot.w * y2;
        wz = rot.w * z2;

        // Note.  Gamasutra gets the matrix assignments inverted, resulting
        // in left-handed rotations, which is contrary to OpenGL and OSG's
        // methodology.  The matrix assignment has been altered in the next
        // few lines of code to do the right thing.
        // Don Burns - Oct 13, 2001
        m[0][0] = 1.0 - (yy + zz);
        m[1][0] = xy - wz;
        m[2][0] = xz + wy;
        m[3][0] = 0.0;


        m[0][1] = xy + wz;
        m[1][1] = 1.0 - (xx + zz);
        m[2][1] = yz - wx;
        m[3][1] = 0.0;

        m[0][2] = xz - wy;
        m[1][2] = yz + wx;
        m[2][2] = 1.0 - (xx + yy);
        m[3][2] = 0.0;
        
        m[0][3] = 0.0;
        m[1][3] = 0.0;
        m[2][3] = 0.0;
        m[3][3] = 1.0;
}

void main()
{
	vec4 pos = texture(postexture, tcoord);
	vec4 n_d = texture(normtexture, tcoord);
	vec3 norm = n_d.xyz;
	float depth = n_d.w;
	vec3 view = -pos.xyz;
	float diffactor = max(dot(lightDir, norm), 0.0);
	vec3 H = normalize(lightDir + view);
	float specfactor = pow(max(dot(H, norm), 0.0), shininess);
	vec4 color = vec4(diffactor);
	
	ivec3 offsetCoord;
	offsetCoord.xy = ivec2(mod(gl_FragCoord.xy, OffsetTexSize.xy));
	float accessibility = 0;
	vec2 curCoord = tcoord;
	float depthP = depth * farDis;
	
	if (bool(useSSAO))
	{
		for (int i = 0; i < int(OffsetTexSize.z); ++i)
		{
			offsetCoord.z = i;
			vec4 offsets = texelFetch(offsettexture, offsetCoord, 0) * aoDensity;
			vec3 posS = vec3(curCoord, depthP) + offsets.xyz;
			float depthS = texture(normtexture, posS.xy).w * farDis;
			
			float frangeIsInvalid = clamp((depthP - depthS) * contrasty, 0.0, 1.0);
			
			if (posS.z < depthS)
				accessibility += 1.0;
			else
				accessibility += mix(0.5, 0.0, frangeIsInvalid);
		}
	}
	else
	{
		vec4 q;
		mat4 m;
		makeRotateToQuad(vec3(0.0, 0.0, 1.0), norm, q);
		getRotateMat(q, m);
		norm = (m * vec4(norm, 1.0)).xyz;
		for (int i = 0; i < int(OffsetTexSize.z); ++i)
		{
			offsetCoord.z = i;
			vec4 offsets = texelFetch(offsettexture, offsetCoord, 0) * aoDensity;
			vec3 posS1 = (m * vec4(offsets.xy, 0.0, 1.0)).xyz;
			vec3 posS2 = (m * vec4(offsets.zw, 0.0, 1.0)).xyz;
			posS1 += vec3(curCoord, depthP);
			posS2 += vec3(curCoord, depthP);
			float depthS1 = texture(normtexture, posS1.xy).w * farDis;
			float depthS2 = texture(normtexture, posS2.xy).w * farDis;
			
			float frangeIsInvalid = clamp((depthP - depthS1) * contrasty, 0.0, 1.0);
			if (posS1.z < depthS1)
				accessibility += 1.0;
			else
				accessibility += mix(0.5, 0.0, frangeIsInvalid);
				
			frangeIsInvalid = clamp((depthP - depthS2) * contrasty, 0.0, 1.0);
			if (posS2.z < depthS2)
				accessibility += 1.0;
			else
				accessibility += mix(0.5, 0.0, frangeIsInvalid);
		}
	
	}
	
	float visibility = accessibility / OffsetTexSize.z;
	FragColor = vec4(visibility * visibility + visibility); 
}