////////////////////////////////////////////////////////////////////////////////

// ---------- vs_tint_lambda.hpp ----------
/*!
\file vs_tint_lambda.hpp
\brief
\author Gareth Edwards
*/


// ---- require:
	//
	//	 INT gid
	//   INT index
	//   vs_system::TintItem li[]
	//


// ---- lambda ------------------------------------------------------------


	// ---- add ------------------------------------------------------------
		auto add_item_with_text = [&](CHAR *name)
			{
				li[++index].Set(name, gid, index);
			};
		auto add_item = [&]()
			{
				li[++index].Set("",gid, index);
			};


		auto add_item_with_text_old = [&](CHAR *name, INT i)
		{
			li[++index].Set(name, gid, i);
		};
		auto add_item_old = [&](INT i)
		{
			li[++index].Set("", gid, i);
		};



	// ---- get ------------------------------------------------------------
		auto get_index = [&]()
		{
			return index;
		};
		auto get_number_of_items = [&]()
		{
			return index + 1;
		};


	// ---- set ------------------------------------------------------------
		auto set_item_tarc = [&](
				INT type,
				INT aid,
				INT row,
				INT col
			)
			{
				li[++index].SetType(type);
				li[index].SetAtlas(aid, row, col);
			};
		auto set_item_tarcx = [&](
				INT type,
				INT aid,
				INT row,
				INT col,
				FLOAT xoff
			)
			{
				li[++index].SetType(type);
				li[index].SetAtlas(aid, row, col);
				li[index].SetXOff(xoff);
			};
		auto set_item_tarcxl = [&](
				INT type,
				INT aid,
				INT row,
				INT col,
				FLOAT xoff,
				BOOL lf
			)
			{
				li[++index].SetType(type);
				li[index].SetAtlas(aid, row, col);
				li[index].SetXOff(xoff);
				li[index].SetLF(lf);
			};
		auto set_item_tarcxli = [&](
				INT type,
				INT aid,
				INT row,
				INT col,
				FLOAT xoff,
				BOOL lf,
				FLOAT lfi
			)
			{
				li[++index].SetType(type);
				li[index].SetAtlas(aid, row, col);
				li[index].SetXOff(xoff);
				li[index].SetLF(lf);
				li[index].SetLFI(lfi);
			};


	// ---- reset ------------------------------------------------------------
		auto reset_index = [&]()
		{
			index = -1;
		};


