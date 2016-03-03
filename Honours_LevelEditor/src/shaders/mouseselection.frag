#version 120

uniform int name;

// Convert from a single int id to an id consisting of 3 0-255 ints
vec4 getname( int n )
{
	vec4 col;
	{
		// Largest -
		col.r = floor( n / ( 255.0f * 255 ) );

		// Medium - Subtract the value of the largest to get the appropriate
		col.g = floor( ( n / ( 255.0f ) ) - ( col.r * 255 ) );

		// Smallest - Subtract the values of the other two to get the appropriate
		col.b = floor( n - ( col.r * 255 * 255 ) - ( col.g * 255 ) );

		col.a = 255;
	}
	return col;
}

void main()
{
	gl_FragColor = getname( name ) / 255.0f;
}