

	// ---- surface stuff
		struct SurfaceShaderData
		{
			VsVector3D  *light;
			double       intensity;
			VsVector3D   vertex;
			VsVector3D   normal;
		};
		struct SurfaceSolutionData
		{
			double       pixel_value;
			VsVector3D  *solution_1;
			VsVector3D  *solution_2;
		};


	// ---- surface 1
		double GetShaderValue_WNA(
			SurfaceShaderData *
		);
		double GetShaderSolution_WNA(
			SurfaceShaderData *,
			SurfaceSolutionData *
		);
		INT SetShaderParameters_WNA();
		double GetShaderHeight_WNA();

		INT SetupSurfaceDisplay1();
		INT SetupSurfaceObjects1();


	// ---- surface 2
		double GetShaderValue2(
				SurfaceShaderData *
			);
		double GetShaderValueUsingCalculatedNormal2(
				SurfaceShaderData *,
				SurfaceSolutionData *
			);
		INT InitSurfaceSceneParam2();
		INT InitSurfaceBufferParam2();

		INT SetupSurfaceDisplay2();
		INT SetupSurfaceObjects2();
