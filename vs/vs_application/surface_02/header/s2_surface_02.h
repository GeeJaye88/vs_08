////////////////////////////////////////////////////////////////////////////////

// ---------- s2_surface_02.h ----------
/*!
\file s3_surface_02.h
\brief Interface for the sub-application class.
\author Gareth Edwards
*/

#pragma once

#include "../header/s2_include.h"


namespace model
{ 


	// ---------- Surface_02 class ----------
	class Surface_02
	{

	public:


	// ---- enumerations ----------------------------------------------------

		// ---- object - "s2_surface_02.cpp"

		// ---- cdtor
			Surface_02();
			virtual ~Surface_02();


		// ---- shared Model & Surface_02 constants used in methods:
			//
			//  s2_model_acquire:-
			//  - Model::Acquire_SetupLighting
			//
			//  s2_scene_setup:-
			//  - Surface_02::SetupGraphicsLighting(model::Surface_02::SceneSetupMode mode)
			//  - Surface_02::SetupParamAcquire(INT *offset)
			//    
			enum LightModel
			{
				// values can be either
				//  - VS_LIGHT_DIRECTIONAL or
				//  - VS_LIGHT_POINT
				//
				LIGHT_TYPE = vs_system::VS_LIGHT_DIRECTIONAL
			};


		// ---- Surface_02 constants used in method:
			//
			// Model::Acquire_CalculateDemoLuminafields
			//    
			enum SurfaceShader
			{
				STANDARD    = 0,
				GREYSCALE   = 1,
				DIRECTIONAL = 2,
				SHADER_TYPE = DIRECTIONAL
			};


		// ---- Surface_02 constants used in:
			//
			//  - Surface_02::SetupParamStore
			//
			enum
			{
				TOTAL_PARAM_SETS = 7
			};


		// ---- mesh
			//  - note:
			//       W - Wireframe
			//       S - Solid
			//       U - Untextured
			//       T - Textured
			//
			enum MeshDisplayMode
			{
				WU = 0,
				SU = 1,
				WT = 2,
				ST = 3
			};
			enum MeshDensityMode
			{
				LOW    = 0,
				MEDIUM = 1,
				HIGH   = 2
			};


	// ---- framework ----------------------------------------------------


		// ---- framework - "s2_surface_02.cpp"
			INT Setup(vs_system::AppDependency*);
			INT SetupGraphics(vs_system::AppKamera *kamera);
			INT SetupDisplay(vs_system::AppKamera *kamera);
			INT CleanupGraphics();
			INT Cleanup();


		// ---- dx fonts
			INT Display3DText(vs_system::AppKamera *kamera);


		// ---- sub-framework - "s2_sub_framework.hpp"
			//
			// - inject application object dependencies
			//
			INT SetObjConfig(vs_system::ObjConfig *);
			INT SetImgProps(vs_system::ImgProps *);


		// ---- setup
			INT SetupAcquire(vs_system::AppKamera *kamera);
			INT SetupAcquireParameters();


		// ---- display list
			INT SetupDisplayList();
			INT GetDisplayListHandle(INT *, CHAR *);


		// ----  gfx objects
			INT SetVisibleObjects(INT, Surface_02 *);
			INT SetVisibleMeshType(INT, Surface_02 *, BOOL);


	// ---- demo source bump maps ----------------------------------------------------


		// ---- bump map - "s2_sub_framework.hpp"
			VOID SetRRBumpMapIndex(UINT);
			UINT GetRRBumpMapIndex();
			INT  GetRRBumpMapData(UCHAR **, INT *);

			INT  GetTerainBumpMapData(UCHAR **, INT *);


	// ---- configuration ----------------------------------------------------


		// ---- pimp configuration facade - "s2_pi_configuration.hpp"


		// ---- acquire surface dimensions
			VOID  SetAcquireSurfaceDimensions(FLOAT, FLOAT, FLOAT);
			VOID  GetAcquireSurfaceDimensions(FLOAT *, FLOAT *, FLOAT*);
			VOID  SetAcquireSurfaceSize(FLOAT);
			FLOAT GetAcquireSurfaceSize();
			VOID  SetAcquireSurfaceYScalar(FLOAT);
			FLOAT GetAcquireSurfaceYScalar();
			FLOAT GetAcquireSetupSourceSurfaceSize(UINT);
			FLOAT GetAcquireSetupSourceSurfaceYScalar(UINT);


		// ---- acquire atlas page
			VOID  SetAcquireAtlasPageLid(UINT);
			UINT  GetAcquireAtlasPageLid();

			VOID  SetAcquireAtlasPageWidth(UINT);
			VOID  SetAcquireAtlasPageHeight(UINT);
			VOID  SetAcquireAtlasPageBytes(UINT);
			UINT  GetAcquireAtlasPageWidth();
			UINT  GetAcquireAtlasPageHeight();
			UINT  GetAcquireAtlasPageBytes();


		// ---- acquire camera
			VOID  SetAcquireCameraWidth(UINT);
			VOID  SetAcquireCameraHeight(UINT);
			VOID  SetAcquireCameraBitdepth(UINT);
			VOID  SetAcquireCameraRect(VsIntRectangle *);
			UINT  GetAcquireCameraWidth();
			UINT  GetAcquireCameraHeight();
			UINT  GetAcquireCameraBitdepth();
			UINT  GetAcquireCameraRectWidth();
			UINT  GetAcquireCameraRectHeight();
			VOID  GetAcquireCameraRect(VsIntRectangle *);


		// ---- acquire unsigned short buffers
			INT   GetAcquireLuminaBuffer(UINT ush_bufer_index, vs_system::UnsignedShortBuffer **ush_buffer);
			INT   SetAcquireLuminaBuffer(UINT ush_bufer_index, vs_system::UnsignedShortBuffer *ush_buffer);
			UINT  GetAcquireLuminaBufferMax();


		// ---- acquire image buffers
			INT   GetAcquireShaderBuffer(UINT img_bufer_index, vs_system::ImgBuffer **img_buffer);
			INT   SetAcquireShaderBuffer(UINT img_bufer_index, vs_system::ImgBuffer *img_buffer);
			UINT  GetAcquireShaderBufferMax();


		// ---- analyse image buffers
			INT   GetAcquireAnalysisBuffer(UINT img_bufer_index, vs_system::ImgBuffer **img_buffer);
			INT   SetAcquireAnalysisBuffer(UINT img_bufer_index, vs_system::ImgBuffer *img_buffer);
			UINT  GetAcquireAnalysisBufferMax();


		// ---- acquire surface version and mode
			UINT  GetAcquireSurfaceVersionSelected();
			VOID  SetAcquireSurfaceVersionSelected(UINT);
			UINT  GetAcquireSurfaceModeSelected();
			VOID  SetAcquireSurfaceModeSelected(UINT);


		// ---- acquire light param
			VOID  SetAcquireLightParam(
					FLOAT azimuth,
					FLOAT elevation,
					FLOAT distance,
					FLOAT intensity,
					FLOAT scalar
				);
			FLOAT GetAcquireLightAzimuthParam();
			FLOAT GetAcquireLightDistanceParam();
			FLOAT GetAcquireLightElevationParam();
			FLOAT GetAcquireLightIntensityParam();
			FLOAT GetAcquireLightScalarValue();


		// ---- acquire light
			UINT  GetAcquireLightMax();
			UINT  GetAcquireLightIndex();
			INT   SetAcquireLightIndex(UINT light_index);
			INT   GetAcquireLight(UINT light_index, vs_system::VsLight **light);
			INT   SetAcquireLight(UINT light_index, vs_system::VsLight *light);

			BOOL  GetAcquireLightEnabled(UINT);
			INT   SetAcquireLightEnabled(UINT);

			BOOL  GetAcquireLightColouredEnabled();
			VOID  SetAcquireLightColouredEnabled(BOOL);
			BOOL  GetAcquireLightSpecularEnabled();
			VOID  SetAcquireLightSpecularEnabled(BOOL);

		// ---- acquire material
			INT   GetAcquireMaterial(vs_system::VsMaterial **material);
			INT   SetAcquireMaterial(vs_system::VsMaterial *material);


		// ---- acquire heightfield param passed to Object CongfigVertex callback
			VOID  SetAcquireHeightfieldParam(UINT stride, FLOAT *buffer, FLOAT scalar);


		// ---- acquire heightfield
			INT   SetAcquireHeightFieldArray(FLOAT * array);
			INT   SetAcquireHeightFieldDimension(UINT dimension);
			INT   SetAcquireHeightFieldExtent(UINT extent);
			INT   SetAcquireHeightFieldStride(UINT stride);
			INT   SetAcquireHeightFieldWidth(UINT width);
			VOID  SetAcquireHeightFieldMin(FLOAT);
			VOID  SetAcquireHeightFieldMax(FLOAT);

			FLOAT *GetAcquireHeightFieldArray();
			UINT  GetAcquireHeightFieldDimension();
			UINT  GetAcquireHeightFieldExtent();
			UINT  GetAcquireHeightFieldStride();
			UINT  GetAcquireHeightFieldWidth();
			FLOAT GetAcquireHeightFieldMin();
			FLOAT GetAcquireHeightFieldMax();

			VOID  Surface_02::SetAcquireHeightFieldScalar(FLOAT);
			FLOAT Surface_02::GetAcquireHeightFieldScalar();


		// ---- explore kamera
			VOID  SetExploreKameraMaxScalarSelected(BOOL);
			VOID  SetExploreKameraMinScalar(FLOAT);
			VOID  SetExploreKameraMaxScalar(FLOAT);

			BOOL  GetExploreKameraMaxScalarSelected();
			FLOAT GetExploreKameraMinScalar();
			FLOAT GetExploreKameraMaxScalar();


		// --- explore lights
			/*
			INT   GetExploreLightMax();
			INT   GetExploreLight(UINT light_index, vs_system::VsLight **light);
			INT   SetExploreLight(UINT light_index, vs_system::VsLight *light);
			BOOL  GetExploreLightEnabled(UINT);
			INT   SetExploreLightEnabled(UINT, BOOL);
			INT   SetExploreLightEnabled(UINT);
			VOID  GetExploreLightsEnabled(BOOL[]);
			VOID  SetExploreLightsEnabled(BOOL[]);
			BOOL  GetExploreLightColouredEnabled();
			VOID  SetExploreLightColouredEnabled(BOOL);
			BOOL  GetExploreLightSpecularEnabled();
			VOID  SetExploreLightSpecularEnabled(BOOL);
			*/

		// ---- mesh modes
			VOID  SetMeshDensity(UINT);
			VOID  SetMeshSolid(BOOL);
			VOID  SetMeshTextured(BOOL);
			UINT  GetMeshDisplayMode();
			VOID  SetMeshDisplayMode(UINT);
			UINT  GetMeshDensityMode();
			VOID  SetMeshDensityMode(UINT);

			UINT  GetMeshColumns();
			UINT  GetMeshRows();
			BOOL  GetMeshSolid();
			BOOL  GetMeshTextured();


	// --- target - "s3_surface_03_config_loi.hpp"

			VOID       InitTargetLoi();

		// ---- get & set
			VOID       SetTargetLoiIndex(UINT);
			UINT       GetTargetLoiIndex();
			VOID       SetTargetLoiInit(BOOL);
			BOOL       GetTargetLoiInit();
			VsVector3 *GetTargetLoi();
			VsVector3 *GetTargetLoi(UINT);
			FLOAT      GetTargetLoiRotation(UINT);
			VOID       SetTargetLoiRulerVisible(BOOL);
			BOOL       GetTargetLoiRulerVisible();
			VOID       SetTargetLoiTranslate(BOOL);
			BOOL       GetTargetLoiTranslate();
			VOID       SetTargetLoiVisible(BOOL);
			BOOL       GetTargetLoiVisible();


		// --- substantive get & set
			FLOAT      CalculateTargetLoi_Surface_Y(UINT);
			FLOAT      CalculateTargetLoi_Surface_Y_Scalar();
			VOID       SetTargetLoiIncr(FLOAT, FLOAT, FLOAT);
			BOOL       CalculateTargetLoi_Surface_Line_Edge_Intersect(
								DOUBLE x1, DOUBLE y1, // input - line 1 start
								DOUBLE x2, DOUBLE y2, //       - line 1 end
								DOUBLE x3, DOUBLE y3, //       - line 2 start
								DOUBLE x4, DOUBLE y4, //       - line 2 end
								DOUBLE &ixOut,        // output - start
								DOUBLE &iyOut         //        - end
							);
			VOID       CalculateTargetLoi_Surface_Section_Intersect();


	// ---- utilty ----------------------------------------------------


		// ---- update all heightfield dependant gfx objects - "s2_obj_heightfield"
			static INT UpdateHeightfieldObjects(INT, Surface_02 *, BOOL);


	// ---- gfx objects ----------------------------------------------------


		// --- explore spot - "s3_obj_exp_lgt_spot.hpp"
			static INT ObjectExpSpot_Setup(INT, Surface_02 *);
			static INT ObjectExpSpot_Display(INT, Surface_02 *, BOOL);

		// --- explore axis - "s3_obj_exp_lgt_axis.hpp"
			static INT ObjectExpLgtAxis_Setup(INT, Surface_02 *);
			static INT ObjectExpLgtAxis_Display(INT, Surface_02 *, BOOL);
			static INT ObjectExpLgtAxis_UpdateAll(Surface_02 *);
			static INT ObjectExpLgtAxis_Update(INT, Surface_02 *);
			static INT ObjectExpLgtAxis_UpdateParam(INT, Surface_02 *);

		// --- surface axis - "s3_obj_surf_axis.hpp"
			static INT ObjectSurfAxis_Setup(INT, Surface_02 *);
			static INT ObjectSurfAxis_Display(INT, Surface_02 *, BOOL);

		// --- surface base - "s3_obj_surf_base.hpp"
			static INT ObjectSurfBase_Setup(INT, Surface_02 *);
			static INT ObjectSurfBase_Display(INT, Surface_02 *, BOOL);

		// --- surface edge - "s3_obj_surf_edge.hpp"
			static INT ObjectSurfEdge_Setup(INT, Surface_02 *);
			static INT ObjectSurfEdge_Display(INT, Surface_02 *, BOOL);
			static INT ObjectSurfEdge_Update(INT, Surface_02 *);
			static INT ObjectSurfEdge_UpdateParam(INT, Surface_02 *);

		// --- surface mesh - "s3_obj_surf_mesh.hpp"
			static INT ObjectSurfMesh_Setup(INT, Surface_02 *);
			static INT ObjectSurfMesh_Display(INT, Surface_02 *, BOOL);
			static INT ObjectSurfMesh_ChangeTexture(INT, Surface_02 *);
			static INT ObjectSurfMesh_Update(INT, Surface_02 *);
			static INT ObjectSurfMesh_UpdateParam(INT, Surface_02 *);

		// --- surface quad - "s3_obj_surf_quad.hpp"
			static INT ObjectSurfQuad_Setup(INT, Surface_02 *);
			static INT ObjectSurfQuad_Display(INT, Surface_02 *, BOOL);
			static INT ObjectSurfQuad_Update(INT, Surface_02 *);
			static INT ObjectSurfQuad_UpdateParam(INT, Surface_02 *);

		// --- loi spot - "s3_obj_loi_spot.hpp"
			static INT ObjectLoiSpot_Setup(INT, Surface_02 *);
			static INT ObjectLoiSpot_Display(INT, Surface_02 *, BOOL);
			static INT ObjectLoiSpot_HideAll(Surface_02 *);
			static INT ObjectLoiSpot_Reset(Surface_02 *);
			static INT ObjectLoiSpot_Update(Surface_02 *);

		// --- loi axis - "s3_obj_loi_axis.hpp"
			static INT ObjectLoiAxis_Setup(INT, Surface_02 *);
			static INT ObjectLoiAxis_Display(INT, Surface_02 *, BOOL);
			static INT ObjectLoiAxis_HideAll(Surface_02 *);
			static INT ObjectLoiAxis_Reset(Surface_02 *);
			static INT ObjectLoiAxis_Update(Surface_02 *);

		// --- loi rect - "s3_obj_loi_rect.hpp"
			static INT ObjectLoiRect_Setup(INT, Surface_02 *);
			static INT ObjectLoiRect_Display(INT, Surface_02 *, BOOL);
			static INT ObjectLoiRect_HideAll(Surface_02 *);
			static INT ObjectLoiRect_Update(Surface_02 *);

		// --- loi rule - "s3_obj_loi_rule.hpp"
			static INT ObjectLoiRule_Setup(INT, Surface_02 *);
			static INT ObjectLoiRule_Display(INT, Surface_02 *, BOOL);
			static INT ObjectLoiRule_HideAll(Surface_02 *);
			static INT ObjectLoiRule_Update(Surface_02 *);

		// --- acquire axis - "s3_obj_acq_lgt_axis.hpp"
			static INT ObjectAcqAxis_Setup(INT, Surface_02 *);
			static INT ObjectAcqAxis_Display(INT, Surface_02 *, BOOL);
			static INT ObjectAcqAxis_Update(INT, Surface_02 *);
			static INT ObjectAcqAxis_UpdateParam(INT, Surface_02 *);

		// --- acquire spot - "s3_obj_acq_lgt_spot.hpp"
			static INT ObjectAcqSpot_Setup(INT, Surface_02 *);
			static INT ObjectAcqSpot_Display(INT, Surface_02 *, BOOL);
			static INT ObjectAcqSpot_Update(INT, Surface_02 *);
			static INT ObjectAcqSpot_UpdateParam(INT, Surface_02 *);


	// ---- parameters ----------------------------------------------------


		// ---- facade
			INT parameter_top = 0;
			vs_system::Parameter parameter;
			#include "vs/vs_library/header/vs_parameter_methods.h"


		// ----- parameter sets 
			vs_system::Parameter parameter_store[7];


		// ---- parameter setup - "s3_surface_03_setup.hpp"
			INT SetupParamSurface(INT *);
			INT SetupParamWeld(INT *);
			INT SetupParamFunction(INT *);
			INT SetupParamLights(INT *);
			INT SetupParamAcquire(INT *);
			INT SetupParamNull();

		// ---- store a parameter set for each source
			INT SetupParamStore();


	// ---- scene ----------------------------------------------------

			enum SceneSetupMode
			{
				NONE,
				ALL,
				ACQUIRE,
				EXPLORE,
				DEFAULT
			};

		// ---- scene - "s2_scene_setup.hpp"
			INT SetupAcquireLightParam();

			INT SetupExploreCamera(SceneSetupMode mode, vs_system::AppKamera *kamera);
			INT SetupExploreLightParam();
	static  INT SetupExploreMaterials(UINT index, Surface_02 *p_s_03);
			INT SetupExploreMaterials(SceneSetupMode mode);
			INT SetupGraphicsLighting(SceneSetupMode mode);


	private:

		// ---- private implementation
			class  PI_Surface_02;
			PI_Surface_02 *pi_surface_02;

	};


}