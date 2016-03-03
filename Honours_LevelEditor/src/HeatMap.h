#pragma once

// Framework Header
#include "ofMain.h"

// Class for displaying heatmap data on a flat plane
// Matthew Cormack

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

		float Time;

		// Collections of 4 floats per data count
		float* Data;
		float DataCount;

		// The size of individual data points
		float Strength;
};
