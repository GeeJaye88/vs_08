////////////////////////////////////////////////////////////////////////////////

// ---------- vs_auto_str.h ----------
/*!
\file vs_auto_str.h
\brief auto str utilities
\author Gareth Edwards
*/

	// ---- lambda ------------------------------------------------------------
		auto StrCatSlash = [](
				char *dirName,
				int *len,
				const char slash
			)
		{
			if (dirName[(*len)-1] == '\\' || dirName[(*len)-1] == '/') return;
			dirName[(*len)++] = slash;
			dirName[*len] = 0;
		};


	// ---- lambda ------------------------------------------------------------
		auto StrDelFolder = [StrCatSlash](
				char *dirName,
				int *len,
				const char slash
			)
		{
			StrCatSlash(dirName, len, '/');
			(*len) -= 2;
			while (*len > 0 && dirName[*len] != '\\' && dirName[*len] != '/') (*len)--;
			(*len)++;
			dirName[*len] = 0;
		};


	// ---- lambda ------------------------------------------------------------
		auto StrRepChar = [](
				char *name,
				const int len,
				const char in,
				const char out
			)
		{
			for (int i = 0; i < len; i++)
				name[i] = name[i] == in ? out : name[i];
		};


	// ---- lambda ------------------------------------------------------------
		auto StrSplitIntoWords = []
			(
				char *line,                // [in]
				const unsigned int len,    // [in]
				char *word[]               // [out]
			)
		{
			enum {
				QUOTE_STARTED = 2,
				QUOTE_ONGOING = 1,
				QUOTE_STOPPED = 0,
				END_OF_WORD   = 0
			};
			unsigned int pendown    = 0;   // non-functional
			unsigned int word_index = 0;   // arg char *word[] 
			unsigned int quote      = 0;   // three states
			char         lc         = 0;   // last char
			for (unsigned int i = 0; i < len; i++)
			{
				char tc = line[i];
				switch (line[i])
				{
					case '#':
						tc = ' ';
						break;
					case '"':
						quote = quote == QUOTE_STOPPED ? QUOTE_STARTED : QUOTE_STOPPED;
						if (quote == QUOTE_STOPPED) line[i] = END_OF_WORD;
						tc = ' ';
						break;
					default:
						if ( quote != QUOTE_ONGOING )
						{
							quote = quote == QUOTE_STARTED ? QUOTE_ONGOING : quote;
							if (tc != ' ' && lc == ' ')
							{
								word[word_index++] = &line[i];
								pendown = 1;
							}
							else if (tc == ' ' && lc != ' ')
							{
								pendown = 0;
								line[i] = END_OF_WORD;
							}
						}
						break;
				}
				lc = tc;
			}
			return word_index;
		};


