////////////////////////////////////////////////////////////////////////////////

// ---------- r2_model.h ----------
/*!
\file r2_model.h
\brief Interface for the Model class.
\author Gareth Edwards
*/

#pragma once


// ---- model - interface, etc..
#include "r2_include.h"


// ---- library include
#include "../../../vs_library/header/vs_tint.h"
#include "../../../vs_library/header/vs_cam_define.h"
#include "../../../vs_library/header/vs_cam_graphs.h" 
#include "../../../vs_library/header/vs_cam_props.h"
#include "../../../vs_library/header/vs_img_props.h"
#include "../../../vs_library/header/vs_obj_config.h"
#include "../../../vs_library/header/vs_img_buffer.h"
#include "../../../vs_library/header/vs_csv.h"
#include "../../../vs_library/header/vs_lidar_ouster.h"


// ---- system include
#include "../../../vs_system/header/vs_elm_context.h"
#include "../../../vs_system/header/vs_pan_context.h"
#include "../../../vs_system/header/vs_app_view.h"


// ---- namespace
namespace review_02
{


	// ---------- Model class ----------
	class Model
	{

	public:

		// ---- c/dtor
			Model();
			virtual ~Model();


		// ---- framework
			INT Setup(vs_system::AppDependency*);
			INT SetupGraphics();
			INT DisplayBegin();
			INT Display(vs_system::PanelContext *, vs_system::ImgBuffer *);
			INT Display(vs_system::PanelContext *);
			INT DisplayEnd();
			INT CleanupGraphics();
			INT Cleanup();

			INT UpdateAutoPTF();

			INT CaptureAnImage(HWND);
			INT SaveBMP(HWND, HDC, HBITMAP);


		// ---- folder hpp ------------------------------------------------------------


			// ---- r2_old_button_geometry.hpp
				static int Callback_Button_Configure(vs_system::ElmContext*);
				static int Callback_Button_GetVertexBuffer(vs_system::ElmContext*);


			// ---- r2_pano_atlas.hpp
				INT    PanoAtlasPageSetup(std::string &, vs_system::ImgBuffer *);
				UCHAR *PanoAtlasPageLock(INT);
				INT    PanoAtlasPageUnlock(INT);


		// ---- folder hpp_cam ------------------------------------------------------------


			// ---- r2_cam_focus_bar_graph.hpp
				INT Dbz_UpdateFocusBarGraph();

			// ---- r2_cam_focus_bar_graph.hpp
				INT GetCamGraphs(vs_system::CamGraphs **);

			// ---- r2_cam_image.hpp
				INT ReportImageConfig();
				INT SaveImageConfig();

			// ---- r2_cam_obj_config.hpp
				INT ReportObjConfigPanoConfig();
				INT SaveObjectAndCameraConfig();

			// ---- r2_cam_obj_config.hpp, cont...
				INT Model::GetConfigFiles(ConfigFiles **);

			// ---- r2_cam_obj_config.hpp TBD: IN WRONG FILE !!!
				vs_system::ObjConfig *GetObjConfig();

			// ---- r2_cam_props.hpp

				INT SetAppConfigPaf(CHAR *);
				INT GetAppConfigPaf(CHAR *);
				static INT SetAppConfigCameraMode(INT);
				static INT GetAppConfigCameraMode();

				INT GetCamProps(vs_system::CamProps **);
				INT GetPropsConfigPaf(CHAR *);
				INT ReadCamProps(CHAR *);
				INT SaveCamProps();
				INT TestSaveCamProps();

		
		// ---- folder hpp_display_list ------------------------------------------------------------

			// ---- r2_dsp_get_panel_handle.hpp
				INT InitPanelDisplayList(vs_system::PanelContext *);
				INT SetPanelGroupId(vs_system::PanelContext *);

			// ---- r2_dsp_about_dashboard.hpp
				INT SetupAboutDashboard();


			// ---- r2_dsp_camera_dashboard.hpp
				INT SetupCameraDashboard();
				INT SetupCameraDashboardDials();
				INT SetupCameraDashboardIcons();
				INT SetupCameraDashboardText();
				INT SetupCameraDashboardGps();
				INT SetupCameraDashboardSys();

				INT SetupCameraDashboardEnclose(INT *, FLOAT, FLOAT, FLOAT);

				INT UpdateCameraDbz(FLOAT []);
				 
					enum Dbz_Cam_Index
					{
						NULL_VALUE          = -1,
						NUMBER_OF_PARAM     = 20,
						ZERO                = 0,
						EXPOSURE_VAL        = 1,
						EXPOSURE_MIN        = 2,
						EXPOSURE_MAX        = 3,
						FRAMERATE_FPS       = 4,
						FRAMERATE_FTS       = 5,
						FRAMERATE_FLS       = 6,
						FRAMERATE_SECOND    = 7,
						GAIN_MASTER         = 8,
						GAIN_WB_RED         = 9,
						GAIN_WB_GREEN       = 10,
						GAIN_WB_BLUE        = 11,
						ICON_EXPOSURE_MODE  = 12,
						ICON_FRAMERATE_MODE = 13,
						ICON_GAIN_MASTER_MODE = 14,
						GPS_FLAG            = 15,
						GPS_LAT             = 16,
						GPS_LONG            = 17,
						GPS_TIME            = 18,
						GPS_DATE            = 19
					};

					std::string cam_dbz_str_text[4];
					FLOAT cam_dbz_param[Dbz_Cam_Index::NUMBER_OF_PARAM];
					BOOL  cam_dbz_update[Dbz_Cam_Index::NUMBER_OF_PARAM];

			// ---- r2_dsp_gps_dashboard.hpp
				INT UpdateGpsDataStr(CHAR *[]);
				std::string gps_data_str_text[4];

			// ---- r2_dsp_progress_bar.hpp
				INT SetupProgressBar();
				INT UpdateProgBarDisplayList(BOOL, FLOAT);

			// ---- r2_dsp_monitor.hpp
				INT SetupMonitorDisplayList();
				INT SetupMonitorControlDisplayList();
				INT UpdateMonitorDisplayList(FLOAT *, FLOAT *);

			// ---- r2_dsp_nest_button.hpp
				INT SetupNestDisplayList();
				INT UpdateNestDisplayList();

			// ---- r2_dsp_stream.hpp

				// capture - not used
				INT UpdateCaptureDisplayList(UINT, UINT, BOOL);

				INT SetStreamElementDisplay(INT);
					INT SetStreamElementDisplayForPano(INT);
				INT SetupStreamDisplayList();
				INT SetStreamDisplayStatus();
				INT UpdateStreamDisplayList(UINT, UINT, BOOL);
				INT UpdateFrameDisplayList(UINT, UINT, BOOL);
				INT SetStreamOnvifHandlerArgs(
						const std::string,
						const std::string,
						const std::string,
						const std::string
					);

				// shared
				INT SetupSharedHiddenLists();

				// ONVIF
				INT SetRemoteControlStatus(BOOL);


			// ---- r2_dsp_lidar.hpp

				INT SetCsvObject(vs_system::CsvObject *csv_buffer);
				INT SetLidarOuster(vs_system::LidarOuster *lidar_ouster);

				INT Update_PanoLidar(vs_system::CsvObject *csv_buffer);
				INT Display_PanoLidar(UINT *lidar_visible_mode);
				INT Rotate_PanoLidar(UINT *lidar_rotate_mode);


		// ---- folder hpp_gfx ------------------------------------------------------------

			// ---- r2_gfx_get_set.hpp
			INT GetGfxShaderParam(UINT, FLOAT *param);
			INT SetGfxShaderParam(UINT, FLOAT *param);


		// ---- folder hpp_img ------------------------------------------------------------

			// ---- r2_img_props.hpp
				INT GetImgProps(vs_system::ImgProps **);
				INT GetImgPropsConfigPaf(CHAR *);
				INT ReadImgProps(CHAR *);
				INT SaveImgProps();
				INT SetImgPropsShaders();


				INT SetupImgExposureAoi();


		// ---- folder hpp_img_panels ------------------------------------------------------------

			// ---- r2_img_panels.hpp

				// invoked from lambda in Control::UpdateFrameGroup
				INT  Copy_ImgPanel_ByFrameId(UINT, UINT);

				INT  Select_ImgPanel_ByFrameId(vs_system::AppFrame *, UINT, CHAR);

				UINT Get_ImgPanel_ActiveId();
				INT  Get_ImgPanel_Id(CHAR*);
				INT  Get_ImgPanel_GroupRow(UINT *);
				INT  Get_ImgPanel_IndexDisplayList(CHAR*);
				INT  Get_ImgPanel_HandleDisplayList(vs_system::PanelContext *, CHAR*);
				//INT  Get_ImgPanel_Status();

				INT  Set_ImgPanel_ActiveId(UINT);
				INT  Set_ImgPanel_ButtonsId(UINT, UINT); // switches on/off
				INT  Set_ImgPanel_GroupDisplay(BOOL);
				INT  Set_ImgPanel_GroupRow(UINT);
				INT  Set_ImgPanel_NavButtons(UINT);      // NOT USED

				INT  Update_ImgPanel_GroupRow();
				INT  Update_ImgPanel_OverviewMap();
				INT  Update_ImgPanel_Config(UINT);       // key method

				INT  Get_ImgPanels_SwitchedOn(
						BOOL switched_on[],
						UINT size);

				INT  Set_ImgPanels_SwitchedOn(           // switches on/off
						BOOL switched_on);

				INT Set_ImgPanels_AtLeastOneOn();        // invoked by Set_ImgPanel _ButtonsId/_SwitchedOn 

		// ---- folder hpp_ptf_panels ------------------------------------------------------------

			// ---- r2_ptf_panels.hpp
				INT  Set_PanoPanel_FrameSelected(UINT);
				INT  Set_ImgOrPanoPanel_IconSelected(UINT);
				INT  Set_ImgOrPanoPanel_Selected(vs_system::PanelContext *, CHAR *);

			// ---- r2_ptf_frames.hpp
				INT  Set_FrameId(UINT);
				UINT Get_FrameId();
				INT  Set_HomeFrameSelected(UINT);
				UINT Get_HomeFrameSelected();
				INT  Set_FrameIcons(UINT, CHAR *, UINT, UINT, UINT, UINT);


		// ---- folder hpp_review ------------------------------------------------------------

			// ---- r2_review_date_time.hpp

				// convert
				ULONG ConvertDateTimeToMillisecond(UINT(*)[DATETIME_MAX_STACK]);

				// set
				INT SetArchiveSaveStatus(BOOL);
				INT SetStreamingStatus(BOOL);
				INT SetStreamingAndOnvifStatus(BOOL);
				INT SetButtonSwitch(UINT, UINT, BOOL);

				// update
				//
				// Note: was INT UpdateDateTimeList(std::string[DATETIME_MAX_STACK], std::string);
				//
				INT UpdateButtonSwitch(UINT, UINT, BOOL);
				INT UpdateDateTimeList(std::string[], std::string);
				INT UpdateDateTimeList(UINT);
				INT UpdateTimelineList(UINT, UINT[], BOOL, LLONG[], UINT, UINT, LLONG[]);

				// highlight timeline
				INT HighlightTimelineList(UINT, UINT, UINT, UINT, LLONG[]);

				// update
				INT UpdateContextualState(BOOL, UINT, UINT);
				INT UpdateClipState(BOOL, UINT);


			// ---- r2_review_get_set.hpp

				// get
				INT GetDeviceStatus();
				INT GetDisplayListHandleByName(CHAR *);
				INT GetElementHandle(CHAR *);
				INT GetHandleElemImgRect(CHAR *, CHAR *, UINT);
				vs_system::ElmContext * GetElmContext(INT);

				// set
				INT SetElementStuff(INT, UINT, UINT, UINT, UINT);
				INT SetElementProperty(INT, UINT, UINT, UINT, UINT);
				INT SetGroupSwitchedOnOff(INT, UINT);
				INT SetConfigureAllBitmask(BYTE);
				INT SetConfigureBitmask(BYTE, CHAR *);


		// ---- folder hpp_setup ------------------------------------------------------------

			// ---- r2_setup_buttons.hpp
				INT SetupButtons();
				INT SetupCameraFramesButtonsList(UINT, UINT, UINT, UINT);
				INT SetupButtonDisplayLists();

			// ---- r2_setup_img_[...].hpp
				INT SetupImgGeometry();
				INT SetupImgNavigation();

			// ---- r2_setup_panel_props.hpp
				//
				// struct PanelProps
				//
				#include "../hpp_setup/r2_setup_panel_props.hpp"

			// ---- r2_setup_panels.hpp
				INT SetupPanels(vs_system::AppDependency *, vs_system::AppView *);

					INT InitialisePanelParam(vs_system::AppDependency *, PanelProps*);
					INT InitialiseInterfacePanelRects(PanelProps *);
					INT InitialisePanoPanelRects(PanelProps *);
					INT InitialisePhotoPanelRects(PanelProps *);
					INT InitialiseGuiPanelRects(PanelProps *);
					INT InitialiseMediaPanelRects(PanelProps *);

					INT InitialiseQuad1PanelRects(PanelProps *);
					INT InitialiseQuad2PanelRects(PanelProps *);
					INT InitialiseNinePanelPanoRects(PanelProps *);
					INT InitialiseNinePanelImgRects(PanelProps *);
					INT InitialiseImg1PanelRects(PanelProps *);
					INT InitialiseSixteenPanelRects(PanelProps *);


			// ---- r2_setup_pano.hpp
				INT SetupPanoExposureAoi();
				INT SetupPanoFocusAoi();
				INT SetupPanoGeometry();
				

			// ---- r2_setup_stream.hpp

				// hidden
				INT SetupStreamHiddenLists();
				INT SetupStreamHiddenListsForImage();
				INT SetupStreamHiddenListsForPano();

				// visible
				INT SetupStreamVisibleLists();
				INT SetupStreamVisibleListsForImage();
				INT SetupStreamVisibleListsForPano();

			// ---- r2_setup_tint_[...].hpp
				INT SetupTintCam();
				INT SetupTintImg();
				static FLOAT TintGetBuildParam(INT);

			// ---- r2_setup_ui.hpp
				INT SetupUIButtons();
				INT SetupUIGeometry();
			

		// ---- folder hpp_tint_[...] ------------------------------------------------------------

			// ---- tabbed interface camera settings
				#include "../hpp_tint_cam/r2_tint_cam_interface.hpp"

			// ---- tabbed interface image settings
				#include "../hpp_tint_img/r2_tint_img_interface.hpp"

		
		// ---- non private (!!!) implementation

			class PI_Model; PI_Model *pi_model;
			class PI_Context; PI_Context *pi_context;

	private:


		// ---- shortcut to injected dependency
			vs_system::GfxInterface  *gfx;


	};

	class  Model::PI_Model;
	static Model::PI_Model *s_pi_model;



}

