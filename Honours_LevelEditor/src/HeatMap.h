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

		void SetData( float* data, int count = 1 );
		inline float* GetData() { return Data; };
		inline float GetDataCount() { return DataCount; };

		inline void SetStrength( float strength ) { Strength = strength; };
		inline float GetStrength() { return Strength; };
		inline float& GetStrengthReference() { return Strength; };

		inline void SetDuration( float duration ) { Duration = duration; };
		inline float GetDuration() { return Duration; };
		inline float& GetDurationReference() { return Duration; };

		inline void SetTimed( bool timed ) { Timed = timed; };
		inline bool GetTimed() { return Timed; };

	private:
		ofPlanePrimitive Plane;
		ofShader Shader;

		// Heatmap time
		float Time;
		bool Timed;

		// Collections of 4 floats per data count
		float* Data;
		int DataCount;

		// The size of individual data points
		float Strength;

		// The time to keep each data point around on the map for (multiplier)
		float Duration;
};
