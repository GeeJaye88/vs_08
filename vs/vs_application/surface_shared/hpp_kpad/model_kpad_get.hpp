////////////////////////////////////////////////////////////////////////////////

// ---------- kpad_get.hpp ----------
/*!
\file kpad_get.hpp
\brief Interface for Model class Tint get methods
\author Gareth Edwards
*/


FLOAT Model::KpadGetBuildParam(INT id)
{
	switch (id)
	{
		case vs_system::Tint::TINT_XOFFSET : return 0.05f;
		case vs_system::Tint::HEAD_YOFFSET : return 0.07f;
		case vs_system::Tint::BODY_YOFFSET : return 0.22f;

		case vs_system::Tint::COL_HEIGHT   : return 0.50f;
		case vs_system::Tint::COL_WIDTH    : return 0.33f;

		case vs_system::Tint::SPACE_WIDTH  : return 0.1f;
		case vs_system::Tint::SPACE_HEIGHT : return 0.05f;
		case vs_system::Tint::SPACE_MARGIN : return 0.00f;

		case vs_system::Tint::TEXT_WIDTH   : return 0.04f;
		case vs_system::Tint::TEXT_HEIGHT  : return 0.04f;

		case vs_system::Tint::ICON_WIDTH   : return 0.05f;
		case vs_system::Tint::ICON_HEIGHT  : return 0.05f;
		case vs_system::Tint::ITEM_SCALAR  : return 1.0f;

		default:
			break;
	} 
	return VS_ERROR;
}


INT Model::KpadGetBodyDisplayListHandle()
{
	return pi_model->handle_dspl_kpad_body;
}


INT Model::KpadGetHeadDisplayListHandle()
{
	return pi_model->handle_dspl_kpad_head;
}
