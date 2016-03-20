#version 150

#define MAXPOSITIONS 1000

uniform int shouldtime;
uniform float duration;
uniform float time;
uniform float points;
uniform float strength;
uniform vec4 heatposition[MAXPOSITIONS];

in vec4 quadCoord;

out vec4 fragColor;

// Get colour from red (full) to blue (empty)
vec3 heat( float v )
{
	float value = 1.0f - v;
	vec3 heatval = vec3( 0, 0, 0 );
	{
		// Used to darken the whole range
		float influence = 0.5f + ( 0.5f * smoothstep( 0.0f, 0.1f, value ) );
		{
			// First give everything a red tinge
			heatval.r += smoothstep( 0.5f, 0.3f, value );
			// Then give the middle bands a green tinge
			heatval.g += value < 0.3f ? smoothstep( 0.0f, 0.3f, value ) : smoothstep( 1.0f, 0.6f, value );
			// Finally the outer bands get a cold blue tinge
			heatval.b += smoothstep( 0.4f, 0.6f, value );
		}
		heatval *= influence;
	}
	return heatval;
}

void main()
{
	float sum = 0;
	{
		for ( int point = 0; point < points; point++ )
		{
			float timestrength = smoothstep( 1, 0, ( abs( time - heatposition[point].w ) / duration ) * shouldtime );
			sum += quadCoord.w * smoothstep( ( 400 * strength ) + ( sin( time * 2 ) * 5 ), 0, distance( quadCoord.xy, heatposition[point].xy * 100 ) ) * timestrength;
		}
	}
	fragColor = vec4( heat(sum), sum );
}