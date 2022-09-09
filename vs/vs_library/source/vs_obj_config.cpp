////////////////////////////////////////////////////////////////////////////////

// ---------- vs_obj_config.cpp ----------
/*!
\file vs_obj_config.cpp
\brief Implementation of the Obj classes.
\author Gareth Edwards
*/


// ---- for the application preprocessor definition...
//
// e.g. #define VS_CAMERA_05
//
#include "../../vs_application/header/vs_app_setup.h"

#include "../header/vs_obj_config.h"


#ifdef VS_GFX_FLAG


using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---- virtual pan, tilt & zoom
	//
	//  includes class ObjVptf
	//
	#include "../hpp_obj/vs_obj_vptf.hpp"


// ---- surface_02
	#include "../hpp_obj/vs_s2_acq_lgt_axis.hpp"


// ---- surface_03
	#include "../hpp_obj/vs_s3_acq_lgt_axis.hpp"
	#include "../hpp_obj/vs_s3_acq_lgt_spot.hpp"
	#include "../hpp_obj/vs_s3_exp_lgt_axis.hpp"

	// -+- fix loi stuff -+-
	#include "../hpp_obj/vs_s3_loi_axis.hpp"
	#include "../hpp_obj/vs_s3_loi_rect.hpp"
	#include "../hpp_obj/vs_s3_loi_rule.hpp"
	#include "../hpp_obj/vs_s3_loi_sect.hpp"
	#include "../hpp_obj/vs_s3_loi_anno.hpp"

	#include "../hpp_obj/vs_s3_surf_axis.hpp"
	#include "../hpp_obj/vs_s3_surf_mount.hpp"


// ---- objects
	#include "../hpp_obj/vs_image_roi.hpp"

	#include "../hpp_obj/vs_obj_config.hpp"
	#include "../hpp_obj/vs_obj_dotobj.hpp"
	#include "../hpp_obj/vs_obj_grid3d.hpp"
	#include "../hpp_obj/vs_obj_kandinsky.hpp"
	#include "../hpp_obj/vs_obj_mesh.hpp"
	#include "../hpp_obj/vs_obj_mesh_edge.hpp"
	#include "../hpp_obj/vs_obj_mesh_quad.hpp"
	#include "../hpp_obj/vs_obj_sphere.hpp"
	#include "../hpp_obj/vs_obj_wavefront.hpp"

	#include "../hpp_obj/vs_button.hpp"
	#include "../hpp_obj/vs_img_rect.hpp"
	#include "../hpp_obj/vs_panel_grid.hpp"
	#include "../hpp_obj/vs_panel_rect.hpp"
	#include "../hpp_obj/vs_box_cube.hpp"
	#include "../hpp_obj/vs_box_face.hpp"


// ---- star system
	#include "../hpp_obj/vs_mystar_asteroid.hpp"
	#include "../hpp_obj/vs_mystar_asteroid_belt.hpp"
	#include "../hpp_obj/vs_mystar_halo.hpp"
	#include "../hpp_obj/vs_mystar_orbit.hpp"
	#include "../hpp_obj/vs_mystar_rings.hpp"
	#include "../hpp_obj/vs_mystar_ring_system.hpp"
	#include "../hpp_obj/vs_mystar_sphere.hpp"
	#include "../hpp_obj/vs_mystar_starfield.hpp"


// ---- pano
	#include "../hpp_obj/vs_pano_annular.hpp"
	#include "../hpp_obj/vs_pano_annular_mask.hpp"
	#include "../hpp_obj/vs_pano_annular_aoi.hpp"
	#include "../hpp_obj/vs_pano_cylinder.hpp"
	#include "../hpp_obj/vs_pano_cylinder_top_ring.hpp"
	#include "../hpp_obj/vs_pano_exposure_aec_line.hpp"
	#include "../hpp_obj/vs_pano_exposure_aec_tri.hpp"
	#include "../hpp_obj/vs_pano_lidar.hpp"
	#include "../hpp_obj/vs_pano_rect.hpp"
	#include "../hpp_obj/vs_pano_rect_part.hpp"
	#include "../hpp_obj/vs_pano_sphere.hpp"


// ---- pano dashboard
	#include "../hpp_obj/vs_obj_dashboard.hpp"


#endif


////////////////////////////////////////////////////////////////////////////////

