////////////////////////////////////////////////////////////////////////////////

// ---------- r2_setup_tint_get_param.hpp ----------
/*!
\file r2_setup_tint_get_param.hpp
\brief
\author Gareth Edwards
*/

using namespace review_02;


FLOAT Model::TintGetBuildParam(INT id)
{
	switch (id)
	{
		case vs_system::Tint::TINT_XOFFSET : return 0.05f;
		case vs_system::Tint::HEAD_YOFFSET : return 0.08f;
		case vs_system::Tint::BODY_YOFFSET : return 0.24f;

		case vs_system::Tint::COL_HEIGHT   : return 0.50f;
		case vs_system::Tint::COL_WIDTH    : return 0.33f;

		case vs_system::Tint::SPACE_WIDTH  : return 0.1f;
		case vs_system::Tint::SPACE_HEIGHT : return 0.05f;
		case vs_system::Tint::SPACE_MARGIN : return 0.00f;

		case vs_system::Tint::TEXT_WIDTH   : return 0.04f;
		case vs_system::Tint::TEXT_HEIGHT  : return 0.04f;

		case vs_system::Tint::ICON_WIDTH   : return 0.06f;
		case vs_system::Tint::ICON_HEIGHT  : return 0.06f;
		case vs_system::Tint::ITEM_SCALAR  : return 0.8f;

		default:
			break;
	} 
	return VS_ERROR;
}
