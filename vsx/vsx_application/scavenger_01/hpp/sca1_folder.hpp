////////////////////////////////////////////////////////////////////////////////

// ---------- sca1_application.hpp ----------
/*!
\file sca1_application.hpp
\brief 
\author Gareth Edwards
*/


// ---------- Scan ----------
/*!
\brief Scan
\return INT - ERROR <=0 < VS_OK
*/


#pragma once


// ---------- CreateTimeline ----------
/*!
\brief Create a folder timeline
\author Gareth Edwards
*/
struct TimelineFolders
{

	// ---- properties
		std::string archive_path = "";
		std::string path = "";
		std::string mask = "*";

	// ---- counters
		UINT total_folders = 0;

	// ---- timeline
		vs_system::VNode *root_folder = NULL;

	// ---- access
		vs_system::VNode *first_minute_folder = NULL;
		vs_system::VNode *last_minute_folder = NULL;

} timeline_folders;


// ---------- CreateTimeline ----------
/*!
\brief Create a folder timeline
\author Gareth Edwards
*/
INT application::CreateTimeline(
		CHAR *archive_path,
		BOOL  refresh
	)
{

	// ---- local
		INT result = 0;
		timeline_folders.archive_path.assign(archive_path);

	// ---- local
		HANDLE hFind = INVALID_HANDLE_VALUE;
		WIN32_FIND_DATA fdata;
		std:: string fullpath;
		std::stack<std::string> folders;

	// ---- push
		folders.push(archive_path);

	// ---- reset timeline folders counter
		TimelineFolders *tf = &timeline_folders;
		tf->total_folders = 0;


	// ---- recurse until no more folders
	while ( !folders.empty() )
	{

		// ---- init current path from folders stack top
			tf->path = folders.top();

		// ---- add to model
			// std::cout << "T : " << tf->path  << std::endl;
			result = T_AddFolder(refresh);

		// ---- directory path
			fullpath = tf->path + "\\" + tf->mask;

		// ---- pop folders stack
			folders.pop();

		// ---- list this folder
			hFind = FindFirstFile(fullpath.c_str(), &fdata);
			if ( hFind != INVALID_HANDLE_VALUE )
			{
					do
					{
							if ( strcmp(fdata.cFileName, ".") != 0 &&
								strcmp(fdata.cFileName, "..") != 0)
							{
								// ---- folder ? -- push current path onto folders stack
									if ( fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
									{
										//if ( refresh ) printf("Folder: %s\n", fdata.cFileName);
										tf->total_folders++;
										folders.push(tf->path + "\\" + fdata.cFileName);
									}
								// ---- ELSE files
									// which are listed AFTER folders - so exit loop
									else break;
							}
					}
					while ( FindNextFile(hFind, &fdata ) != 0);
			}

	 	// ---- check this folder listing is complete
			if ( GetLastError() != ERROR_NO_MORE_FILES )
			{
				FindClose(hFind);
				return VS_ERROR;
			}

		// ---- close this folder
			FindClose(hFind);
			hFind = INVALID_HANDLE_VALUE;
	}


	// printf("# of Archive folders is %d and took %d ms\n", folder_counter, (INT)total_time);


	// ---- finally, "cross" link folder nodes
		vs_system::VNode *link_stack[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
		UINT link_index = 0;
		result = T_CrossLinkFolders(
				tf->root_folder,
				link_index,
				link_stack
			);


	// ---- report "cross" linking
		BOOL report = TRUE;
		if ( report )
		{
			vs_system::VNode *years   = tf->root_folder->GetFirst();
			vs_system::VNode *months  = years->GetFirst();
			vs_system::VNode *days    = months->GetFirst();
			vs_system::VNode *hours   = days->GetFirst();
			vs_system::VNode *minutes = hours->GetFirst();
			ListCrossLinkFolders(years);
			ListCrossLinkFolders(months);
			ListCrossLinkFolders(days);
			ListCrossLinkFolders(hours);
			ListCrossLinkFolders(minutes);
		}


	// ---- set last folder
		vs_system::VNode *last = tf->root_folder;
		while (last != NULL)
		{
			timeline_folders.last_minute_folder = last;
			last = last->GetLast();
		}


	// IF NOT refresh THEN initialise date time stack
	//if ( !refresh )
	//{
	//	result = img_timeline_folders.SetDateTimeStack(img_timeline_folders.last_minute_folder);
	//}


	// ---- report P(rev) and S(next) cross links
		if ( report )
		{
			// Shortcut
			TimelineFolders *tf = &timeline_folders;

			// IF there is a "last" minute "previous" minute
			vs_system::VNode *prev_min = tf->last_minute_folder->GetLinkP();
			if ( prev_min != NULL )
			{
				printf("KKK prev_min = %s\n", prev_min->GetName());
				printf("    next_min = %s\n", prev_min->GetLinkS()->GetName());
			}
		}

	return VS_OK;
}


// ---------- T_ScanFolders_01 ----------
/*!
\brief Scan timeline folders
\author Gareth Edwards
*/
INT application::T_ScanFolders_01()
{

	// ---- local folder
		INT  result = 0;
		INT  folder_counter = 0;
		CHAR folder_stack[16][VS_MAXCHARPATHLEN];
		INT  folder_stack_top = -1;
		CHAR folder_store_path[VS_MAXCHARPATHLEN];
		CHAR folder_new_path[VS_MAXCHARPATHLEN];
		CHAR folder_list_path[VS_MAXCHARPATHLEN];

	// ---- local windows
		HANDLE hFind = INVALID_HANDLE_VALUE;
		WIN32_FIND_DATA fdata;

	// ---- accelerate timeline folders
		TimelineFolders *tf = &timeline_folders;

	// ---- initialise folder stack
		strcpy(folder_stack[++folder_stack_top], timeline_folders.archive_path.c_str() );


	// Recurse until no more folders
		while ( folder_stack_top >= 0 )
		{
			// ---- store folder path
				strcpy(folder_store_path, folder_stack[folder_stack_top]);

			// ---- create folder listing path
				sprintf(folder_list_path, "%s\\*", folder_store_path);

			// ---- pop folders stack
				folder_stack_top--;

			// ---- list folder
				hFind = FindFirstFile(folder_list_path, &fdata);
				if (hFind != INVALID_HANDLE_VALUE)
				{
						do
						{
								if (strcmp(fdata.cFileName, ".") != 0 &&
									strcmp(fdata.cFileName, "..") != 0)
								{
									// ---- folder ? -- push onto folders stack
										if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
										{
											folder_counter++;
											sprintf(
													folder_new_path,
													"%s\\%s",
													folder_store_path,
													fdata.cFileName
												);
											strcpy(folder_stack[++folder_stack_top], folder_new_path);
										}
									// ELSE files
										// which are listed AFTER folders - so exit loop
										else break;
								}
						} while (FindNextFile(hFind, &fdata) != 0);
				}


			// ---- check this folder listing is complete
				if (GetLastError() != ERROR_NO_MORE_FILES)
				{
					FindClose(hFind);
					printf("Folder Scan: Failed.... %d folders, ERROR_NO_MORE_FILES\n", folder_counter);
					return VS_ERROR;
				}


			// ---- close FindFirstFile
				FindClose(hFind);
				hFind = INVALID_HANDLE_VALUE;

		}

	return folder_counter;
}


// ---------- T_ScanFolders ----------
/*!
\brief Scan timeline folders
\author Gareth Edwards
*/
INT application::T_ScanFolders()
{

	// ---- local
		INT result = 0;

	// ---- local timeline
		TimelineFolders *tf = &timeline_folders;
		std::string tf_path;
		tf_path.assign(timeline_folders.archive_path);

	// ---- local windows
		HANDLE hFind = INVALID_HANDLE_VALUE;
		WIN32_FIND_DATA fdata;

	// ---- local folder
		std::string fullpath;
		std::stack<std::string> folders;
		folders.push(tf_path);
		UINT folder_counter = 0;


	// ---- recurse until no more folders
		while ( !folders.empty() )
		{

			// ---- init current path from folders stack top
			tf_path = folders.top();

			// std::cout << "T : " << tf_path << std::endl;

			// ---- init full directory path..
			fullpath = tf_path + "\\" + tf->mask;

			// ---- then pop folders stack
			folders.pop();


			// ---- list this folder
				hFind = FindFirstFile(fullpath.c_str(), &fdata);
				if (hFind != INVALID_HANDLE_VALUE)
				{
						do
						{
								if (strcmp(fdata.cFileName, ".") != 0 &&
									strcmp(fdata.cFileName, "..") != 0)
								{
									// ---- folder ? -- push current path onto foldersa stack
										if ( fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
										{
											// printf("Folder: %s\n", fdata.cFileName);
											folder_counter++;
											folders.push(tf_path + "\\" + fdata.cFileName);
										}
									// ELSE files
										// which are listed AFTER folders - so exit loop
										else break;
								}
						} while (FindNextFile(hFind, &fdata) != 0);
				}


			// ---- check this folder listing is complete
				if ( GetLastError() != ERROR_NO_MORE_FILES )
				{
					FindClose(hFind);
					printf("Folder Scan: Failed.... %d folders, ERROR_NO_MORE_FILES\n", folder_counter);
					return VS_ERROR;
				}

			// ---- close this folder
				FindClose(hFind);
				hFind = INVALID_HANDLE_VALUE;
		}

	return (INT)folder_counter;
}


// ---------- T_AddFolder  ----------
/*!
\brief Add folder to timeline
\author Gareth Edwards
*/
INT application::T_AddFolder(BOOL refresh)
{

	// ---- split into path component names
		//
		// Note: archive path is comprised of folder/folder/etc.,
		// and by building with backslash this allows the logic
		// that the when split - and there is only one word,
		// then this is the root path.
		//
		vs_system::CharProc cp;
		CHAR temp[VS_MAXCHARLEN];
		strcpy(temp, timeline_folders.path.c_str());
		CHAR *stack[32];
		UINT num_words = cp.Split(stack, 32, '\\', temp);
	

	// ---- ? -- only one word THEN
		if ( num_words == 1 )
		{
			// ? NOT refresh mode -- create root folder
				if ( !refresh )
				{
					// ---- add, allocate and create root node
						UINT len = strlen(stack[0]);
						timeline_folders.root_folder = new vs_system::VNode(stack[num_words - 1]);

				}
		}

	// ELSE --
		// add allocate and create folder nodes
		else
		{
				// ---- FOR each hierarchical level on "word" stack except first...
				vs_system::VNode *parent = timeline_folders.root_folder;
				for ( WORD w=1; w<num_words; w++ )
				{
					// ---- search for matching child name
					vs_system::VNode *child = parent->GetFirst();
					while ( child )
					{
						CHAR *child_name = child->GetName();
						if ( !strcmp(child_name, stack[w]) )
							break;
						child = child->GetNext();
					}

					// ---- ? last hierarchical level on "word" stack AND
						// search for child by named fails THEN
						// add new node, etc.... AND return
						//
						if ( w==num_words-1 && child==NULL )
						{
							vs_system::VNode* new_node = NULL;
							if ( refresh )
							{
								new_node = new vs_system::VNode(stack[w], parent, 1);
								// printf("Append minute %s\n", new_node->GetName());
							}
							else
							{
								new_node = new vs_system::VNode(stack[w], parent);
							}
							return VS_OK;
						}

					// ---- next hierarchical level on "word" stack
						parent = child;

				}

		}

    return VS_OK;
}


// ---------- T_CrossLinkFolders ----------
/*!
\brief Recursively cross link folders
\author Gareth Edwards
*/
INT application::T_CrossLinkFolders(
		vs_system::VNode *node,
		UINT link_index,
		vs_system::VNode *link[]
	)
{

	// ---- ? leaf -- set "first_minute_folder"
		if ( node->GetFirst() == NULL )
			if ( timeline_folders.first_minute_folder == NULL )
				 timeline_folders.first_minute_folder = node;


	// ---- ? previous -- set previous "cross" link to "this" node
		vs_system::VNode *previous = link[link_index];
		if ( previous != NULL)
		{
			// ---- link from previous to "this"
			previous->SetLinkS(node);

			// ---- link from "this back to previous
			node->SetLinkP(previous);

			// ---- terminate
			node->SetLinkS(NULL);
		}


	// ----replace "previous" with "this"
		link[link_index] = node;


	// ---- recurse
		vs_system::VNode *child = node->GetFirst();
		while ( child )
		{
			INT result = T_CrossLinkFolders(child, link_index+1, link);
			child = child->GetNext();
		}

    return VS_OK;
}


// ---------- ListAllFolders ----------
/*!
\brief List all folders
\author Gareth Edwards
*/
INT application::ListAllFolders(vs_system::VNode *parent)
{

	vs_system::VNode *grandparent = parent->GetParent();
	if ( grandparent )
		std::cout << grandparent->GetName() << " <- " << parent->GetName() << std::endl;
	else
		std::cout << parent->GetName() << std::endl;
	vs_system::VNode *child = parent->GetFirst();
	while ( child )
	{
		INT result = ListAllFolders(child);
		child = child->GetNext();
	}

	return VS_OK;
}


// ---------- ListCrossLinkFolders ----------
/*!
\brief List all folders
\author Gareth Edwards
*/
INT application::ListCrossLinkFolders(vs_system::VNode *node)
{

	if ( node->GetParent() != NULL ) std::cout << node->GetParent()->GetName() << " : ";
	while ( node )
	{
		vs_system::VNode *link_p = node->GetLinkP();
		vs_system::VNode *link_s = node->GetLinkS();

		char temp[256];
		sprintf(
				temp,
				"%s<-%s->%s",
				(link_p == NULL ? "N" : link_p->GetName() ),
				node->GetName(),
				(link_s == NULL ? "N" : link_s->GetName() )
			);

		std::cout << temp << (link_s == NULL ? " " : ", ");
		node = link_s;
	}
	std::cout << std::endl;

	return VS_OK;
}
