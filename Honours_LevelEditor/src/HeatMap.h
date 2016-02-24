#pragma once

#include "ofMain.h"

class HeatMapClass
{
	public:
		HeatMapClass();
		~HeatMapClass();

		void Initialize();
		void Update( float time );
		void Draw();

		void SetData( float* data, float count = 1 );
		inline float* GetData() { return Data; };
		inline float GetDataCount() { return DataCount; };

		void SetStrength( float strength ) { Strength = strength; };
		inline float GetStrength() { return Strength; };
		inline float& GetStrengthReference() { return Strength; };

	private:
		ofPlanePrimitive Plane;
		ofShader Shader;

		// Collections of 4 floats per data count
		float* Data;
		float DataCount;

		// The size of individual data points
		float Strength;
};
