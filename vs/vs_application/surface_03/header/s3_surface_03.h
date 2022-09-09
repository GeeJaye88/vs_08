////////////////////////////////////////////////////////////////////////////////

// ---------- s3_surface_03.h ----------
/*!
\file s3_surface_03.h
\brief Interface for the sub-application class.
\author Gareth Edwards
*/

#pragma once

#include "../header/s3_include.h"


namespace model
{ 


	// ---------- Surface_03 class ----------
	class Surface_03
	{

	public:


		// ---- cdtor
			Surface_03();
			virtual ~Surface_03();


		// ---- acquire
			enum Acquire
			{
				TOTAL_IMG_BUFFER = 5,
				TOTAL_LIGHT = 4
			};


		// ---- mesh
		//      note:
		//         W - Wirefram
		//         S - Solid
		//         U - Untextured
		//         T - Textured
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


		// ---- framework
			INT Setup(vs_system::AppDependency*);
			INT SetupGraphics(vs_system::AppKamera *kamera);
			INT SetupDisplay(vs_system::AppKamera *kamera);
			INT CleanupGraphics();
			INT Cleanup();


		// ---- sub-framework
			INT SetObjConfig(vs_system::ObjConfig *);
			INT SetImgProps(vs_system::ImgProps *);


		// ---- dx fonts
			INT Display3DText(vs_system::AppKamera *kamera);


		// ---- display list
			INT SetupDisplayList();
			INT GetDisplayListHandle(INT *, CHAR *);


		// --- Begin "s3_surface_03_config_loi.hpp"


		// ---- Part 1: Acquire


		// ---- Acquire Atlas Pages
			VOID  SetAcquireAtlasPageLid(UINT);
			UINT  GetAcquireAtlasPageLid();
			VOID  SetAcquireAtlasPageWidth(UINT);
			VOID  SetAcquireAtlasPageHeight(UINT);
			VOID  SetAcquireAtlasPageBytes(UINT);
			UINT  GetAcquireAtlasPageWidth();
			UINT  GetAcquireAtlasPageHeight();
			UINT  GetAcquireAtlasPageBytes();


		// ---- Acquire Surface Dimensions
			VOID  SetAcquireSurfaceDimensions(FLOAT, FLOAT, FLOAT);
			VOID  GetAcquireSurfaceDimensions(FLOAT *, FLOAT *, FLOAT*);
			FLOAT GetAcquireSurfaceLimit();
			FLOAT GetAcquireSurfaceSize();

		// ---- Acquire Light Parameters
			VOID  SetAcquireLightParam(
					FLOAT azimuth,
					FLOAT elevation,
					FLOAT distance,
					FLOAT intensity
				);
			FLOAT GetAcquireLightAzimuthParam();
			FLOAT GetAcquireLightDistanceParam();
			FLOAT GetAcquireLightElevationParam();
			FLOAT GetAcquireLightIntensityParam();


		// ---- Acquire Lights
			INT   GetAcquireLight(UINT light_index, vs_system::VsLight **light);
			INT   SetAcquireLight(UINT light_index, vs_system::VsLight *light);


		// ---- Acquire Height Field Parameters
			//
			// Note: passed to Object CongfigVertex callback
			//
			VOID  SetAcquireHeightfieldParam(UINT stride, FLOAT *buffer, FLOAT scalar);


		// ---- Acquire Height Field Array Parameters
			INT   Surface_03::SetAcquireHeightFieldArray(FLOAT * array);
			INT   Surface_03::SetAcquireHeightFieldDimension(UINT dimension);
			INT   Surface_03::SetAcquireHeightFieldExtent(UINT extent);
			INT   Surface_03::SetAcquireHeightFieldOffset(UINT dimension);
			INT   Surface_03::SetAcquireHeightFieldStride(UINT pitch);
			INT   Surface_03::SetAcquireHeightFieldWidth(UINT width);
			FLOAT *Surface_03::GetAcquireHeightFieldArray();
			UINT  Surface_03::GetAcquireHeightFieldDimension();
			UINT  Surface_03::GetAcquireHeightFieldExtent();
			UINT  Surface_03::GetAcquireHeightFieldOffset();
			UINT  Surface_03::GetAcquireHeightFieldStride();
			UINT  Surface_03::GetAcquireHeightFieldWidth();
			VOID  Surface_03::SetAcquireHeightFieldMin(FLOAT);
			FLOAT Surface_03::GetAcquireHeightFieldMin();
			VOID  Surface_03::SetAcquireHeightFieldMax(FLOAT);
			FLOAT Surface_03::GetAcquireHeightFieldMax();


		// ---- Acquire Height Field Scalar
			VOID  Surface_03::SetAcquireHeightFieldScalar(FLOAT);
			FLOAT Surface_03::GetAcquireHeightFieldScalar();


		// ---- Acquire Image Buffers
			INT   GetAcquireImgBuffer(UINT img_bufer_index, vs_system::ImgBuffer **img_buffer);
			INT   SetAcquireImgBuffer(UINT img_bufer_index, vs_system::ImgBuffer *img_buffer);
			UINT  GetAcquireImgBufferMax();


		// ---- Analyse Image Buffers
			INT   GetAnalyseImgBuffer(UINT img_bufer_index, vs_system::ImgBuffer **img_buffer);
			INT   SetAnalyseImgBuffer(UINT img_bufer_index, vs_system::ImgBuffer *img_buffer);


		// ---- Acquire Material
			INT   GetAcquireMaterial(vs_system::VsMaterial **material);
			INT   SetAcquireMaterial(vs_system::VsMaterial *material);


		// ---- Part 2: Explore


		// --- Explore Lights
			INT   GetExploreLightMax();
			INT   GetExploreLight(UINT light_index, vs_system::VsLight **light);
			BOOL  GetExploreLightEnabled(UINT);
			INT   SetExploreLightEnabled(UINT, BOOL);
			VOID  GetExploreLightsEnabled(BOOL[]);
			VOID  SetExploreLightsEnabled(BOOL[]);
			BOOL  GetExploreLightColouredEnabled();
			VOID  SetExploreLightColouredEnabled(BOOL);
			BOOL  GetExploreLightSpecularEnabled();
			VOID  SetExploreLightSpecularEnabled(BOOL);


		// ---- Mesh Modes
			VOID  SetMeshDensity(UINT);
			VOID  SetMeshSolid(BOOL);
			VOID  SetMeshTextured(BOOL);
			UINT  GetMeshDisplayMode();
			VOID  SetMeshDisplayMode(UINT);
			UINT  GetMeshDensityMode();
			VOID  SetMeshDensityMode(UINT);
			UINT  GetMeshMode();
			VOID  SetMeshMode(UINT);
			BOOL  GetMeshSolid();
			BOOL  GetMeshTextured();


		// ---- Mesh Dimensions
			UINT  GetMeshColumns();
			UINT  GetMeshRows();


	// --- Part 3: Target LOI

			VOID       InitTargetLoi();

		// ---- Get & Set
			UINT       GetTargetLoiMax();

		// ---- Index
			VOID       SetTargetLoiIndex(UINT);
			UINT       GetTargetLoiIndex();
			UINT       GetTargetLoiIndexPrev();

		// ---- Initialisation
			VOID       SetTargetLoiInit(BOOL);
			BOOL       GetTargetLoiInit();

		// ---- Ordinates
			FLOAT      GetTargetLoi(UINT, CHAR);
			INT        SetTargetLoi(UINT, CHAR, FLOAT);
			VOID       GetTargetLoi(UINT, VsVector3 *v);
			VOID       SetTargetLoi(UINT, VsVector3 *v);

		// ---- Ordinates for Display
			VOID       GetTargetLoiForDisplay(VsVector3 *v);
			FLOAT      GetTargetLoiValueForDisplay(UINT, FLOAT);
			FLOAT      GetTargetLoiInverseForDisplay(UINT, FLOAT);

		// ---- Ordinates - rotation of section
			FLOAT      GetTargetLoiRotation(UINT);

		// ---- Display Target Objects
			VOID       SetTargetLoiVisible(BOOL);
			BOOL       GetTargetLoiVisible();
			VOID       SetTargetLoiRulerVisible(BOOL);
			BOOL       GetTargetLoiRulerVisible();
			VOID       SetTargetLoiSectionVisible(BOOL);
			BOOL       GetTargetLoiSectionVisible();
			VOID       SetTargetLoiProfileVisible(BOOL);
			BOOL       GetTargetLoiProfileVisible();
			BOOL       GetTargetLoiAnnotationVisible();

		// ---- Surface
			FLOAT      GetTargetLoiSurfaceY();
			VOID       SetTargetLoiSurfaceY(FLOAT);

		// ---- Section Parameters
			VOID       SetTargetLoiSectionLength(FLOAT);
			FLOAT      GetTargetLoiSectionLength();
			VOID       SetTargetLoiSectionHeight(FLOAT);
			FLOAT      GetTargetLoiSectionHeight();

		// ---- Section  Modes
			VOID       SetTargetLoiTargetMode(BOOL);
			BOOL       GetTargetLoiTargetMode();
			VOID       SetTargetLoiSectionMode(BOOL);
			BOOL       GetTargetLoiSectionMode();

		// ---- Measure Mode
			VOID       SetTargetLoiMeasure(BOOL);
			BOOL       GetTargetLoiMeasure();

		// ---- Measure Vertical FoV (not used)
			VOID       SetTargetLoiMeasureFov(FLOAT);
			FLOAT      GetTargetLoiMeasureFov();


		// --- Part 4: Substantive Calculations

			INT        CalculateTargetLoi_Setup();
			INT        CalculateTargetLoi_Setup_Hexagon();

			FLOAT      CalculateTargetLoi_Surface_Y(UINT);
			INT        CalculateTargetLoi_Surface_Section(
								DOUBLE x1, DOUBLE y1, // - start
								DOUBLE x2, DOUBLE y2, // - end
								UINT   num_samples,   // - number of mesh samples
								FLOAT  *samples       // section y samples
							);
			FLOAT      CalculateTargetLoi_Surface_Y_Scalar();

			BOOL       CalculateTargetLoi_Surface_Increment(FLOAT, FLOAT, FLOAT, BOOL);
			BOOL       CalculateTargetLoi_Rotation_Increment(FLOAT);

			BOOL       CalculateTargetLoi_Surface_Line_Edge_Intersect(
								DOUBLE x1, DOUBLE y1, // input - line 1 start
								DOUBLE x2, DOUBLE y2, //       - line 1 end
								DOUBLE x3, DOUBLE y3, //       - line 2 start
								DOUBLE x4, DOUBLE y4, //       - line 2 end
								DOUBLE &ixOut,        // output - start
								DOUBLE &iyOut         //        - end
							);
			VOID       CalculateTargetLoi_Surface_Section_Intersect();


	// --- End of "s3_surface_03_config_loi.hpp"


	// ---- selected objects

			static INT UpdateHeightfieldObjects(INT, Surface_03 *, BOOL);
			static INT UpdateLoiObjects(INT, Surface_03 *);
			static INT SetVisibleObjects(INT, Surface_03 *);


	// ---- objects

		// --- explore spot - "s3_obj_exp_lgt_spot.hpp"
			static INT ObjectExpSpot_Setup(INT, Surface_03 *);
			static INT ObjectExpSpot_Display(INT, Surface_03 *, BOOL);
		
		// --- explore axis - "s3_obj_exp_lgt_axis.hpp"
			static INT ObjectExpLgtAxis_Setup(INT, Surface_03 *);
			static INT ObjectExpLgtAxis_Display(INT, Surface_03 *, BOOL);
			static INT ObjectExpLgtAxis_UpdateAll(Surface_03 *);
			static INT ObjectExpLgtAxis_Update(INT, Surface_03 *);
			static INT ObjectExpLgtAxis_UpdateParam(INT, Surface_03 *);

		// --- surface axis - "s3_obj_surf_axis.hpp"
			static INT ObjectSurfAxis_Setup(INT, Surface_03 *);
			static INT ObjectSurfAxis_Display(INT, Surface_03 *, BOOL);

		// --- surface base - "s3_obj_surf_base.hpp"
			static INT ObjectSurfBase_Setup(INT, Surface_03 *);
			static INT ObjectSurfBase_Display(INT, Surface_03 *, BOOL);
			
		// --- surface edge - "s3_obj_surf_edge.hpp"
			static INT ObjectSurfEdge_Setup(INT, Surface_03 *);
			static INT ObjectSurfEdge_Display(INT, Surface_03 *, BOOL);
			static INT ObjectSurfEdge_Update(INT, Surface_03 *);
			static INT ObjectSurfEdge_UpdateParam(INT, Surface_03 *);

		// --- surface mesh - "s3_obj_surf_mesh.hpp"
			static INT ObjectSurfMesh_Setup(INT, Surface_03 *);
			static INT ObjectSurfMesh_Display(INT, Surface_03 *, BOOL);
			static INT ObjectSurfMesh_Update(INT, Surface_03 *);
			static INT ObjectSurfMesh_UpdateParam(INT, Surface_03 *);

		// ---- loi spot

		// --- loi spot - "s3_obj_loi_spot.hpp"
			static INT ObjectLoiSpot_Setup(INT, Surface_03 *);
			static INT ObjectLoiSpot_Display(INT, Surface_03 *, BOOL);
			static INT ObjectLoiSpot_HideAll(Surface_03 *);
			static INT ObjectLoiSpot_Reset(Surface_03 *);
			static INT ObjectLoiSpot_Update(Surface_03 *);

		// --- loi axis - "s3_obj_loi_axis.hpp"
			static INT ObjectLoiAxis_Setup(INT, Surface_03 *);
			static INT ObjectLoiAxis_Display(INT, Surface_03 *, BOOL);
			static INT ObjectLoiAxis_HideAll(Surface_03 *);
			static INT ObjectLoiAxis_Reset(Surface_03 *);
			static INT ObjectLoiAxis_Update(Surface_03 *);

		// --- loi rect - "s3_obj_loi_rect.hpp"
			static INT ObjectLoiRect_Setup(INT, Surface_03 *);
			static INT ObjectLoiRect_Display(INT, Surface_03 *, BOOL);
			static INT ObjectLoiRect_HideAll(Surface_03 *);
			static INT ObjectLoiRect_Update(Surface_03 *);

		// --- loi rule - "s3_obj_loi_rule.hpp"
			static INT ObjectLoiRule_Setup(INT, Surface_03 *);
			static INT ObjectLoiRule_Display(INT, Surface_03 *, BOOL);
			static INT ObjectLoiRule_HideAll(Surface_03 *);
			static INT ObjectLoiRule_Update(Surface_03 *);

		// --- loi section - "s3_obj_loi_sect.hpp"
			static INT ObjectLoiSect_Setup(INT, Surface_03 *);
			static INT ObjectLoiSect_Display(INT, Surface_03 *, BOOL);
			static INT ObjectLoiSect_HideAll(Surface_03 *);
			static INT ObjectLoiSect_Update(Surface_03 *);

		// --- loi section - "s3_obj_loi_anno.hpp"
			static INT ObjectLoiAnno_Setup(INT, Surface_03 *);
			static INT ObjectLoiAnno_Display(INT, Surface_03 *, BOOL);
			static INT ObjectLoiAnno_HideAll(Surface_03 *);
			static INT ObjectLoiAnno_Update(Surface_03 *);

		// --- acquire axis - "s3_obj_acq_lgt_axis.hpp"
			static INT ObjectAcqAxis_Setup(INT, Surface_03 *);
			static INT ObjectAcqAxis_Display(INT, Surface_03 *, BOOL);
			static INT ObjectAcqAxis_Update(INT, Surface_03 *);
			static INT ObjectAcqAxis_UpdateParam(INT, Surface_03 *);

			
		// ---- parameter & methods included for facade copy
			vs_system::Parameter parameter;
			#include "vs/vs_library/header/vs_parameter_methods.h"


		// --- scene - "s3_surface_03_scene_setup.hpp"
			INT SetupExploreCamera(UINT mode, vs_system::AppKamera *kamera);
			INT SetupAcquireLight();
			INT SetupExploreLighting(UINT mode);
			INT SetupExploreMaterials(UINT mode);
			static INT SetupExploreMaterials(UINT index, Surface_03 *p_s_03);


		// --- parameter setup - "s3_param_setup.hpp"
			INT SetupParamCamera(INT *);
			INT SetupParamHeightMap(INT *);
			INT SetupParamFunction(INT *);
			INT SetupParamLights(INT *);
			INT SetupParamMeasure(INT *);
			INT SetupParamAcquire(INT *);
			INT SetupParamNull();


		// ---- demo image byte data
			VOID SetRRBumpMapIndex(UINT);
			UINT GetRRBumpMapIndex();
			INT  GetRRBumpMapData(UCHAR **, INT *);
			INT  GetTerainBumpMapData(UCHAR **, INT *);

	private:

		// ---- private implementation
			class  PI_Surface_03;
			PI_Surface_03 *pi_surface_03;

	};


}