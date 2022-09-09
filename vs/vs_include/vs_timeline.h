////////////////////////////////////////////////////////////////////////////////

// ---------- vs_timeline.h ----------
/*!

\file vs_timeline.h
\brief Interface for the TimelineFolder class.
\author Gareth Edwards
*/

#pragma once


// ---- include 
#include <stdio.h>        // 
#include <ctype.h>        // 
#include <string>         // std::string
#include <iostream>       // std::cout
#include <stack>          // std::stack
#include <vector>         // std::vector
#include <deque>          // std::deque


// ---- vs include ----
#include "../vs_system/header/vs_inc_defs.h"
#include "../vs_library/header/vs_vnode.h"


// ---------- namespace ----------
namespace vs_system
{


	// ---------- Timeline Info Struct ----------
	struct TimelineInfo
	{

		// ---------- properties ----------


		// ---- archive
			std::string archive_path = "";
			std::string path = "";
			std::string mask = "*";

		// --- folder/file
			std::string file_name;
			std::string folder_name;
			std::string folder_path;

		// ---- counters
			UINT folder_total = 0;
			UINT folder_depth = 0;

		// ---- state
			INT  create_status = VS_OK;

		// ---- timeline
			VNode *root_folder = NULL;

		// ---- access
			#ifdef OLD_TIMELINE
			VNode *first_minute_folder = NULL;
			VNode *last_minute_folder = NULL;
			#endif

		// ---- date time stack
			enum Command {
				REMOVE
			};
			enum DateTime {
				YEAR,
				MONTH,
				DAY,
				HOUR,
				MINUTE
			};
			VNode *datetime[5] = { NULL, NULL, NULL, NULL, NULL };


		// ---------- methods ----------

			VOID Initialise()
			{
				archive_path        = "";
				path                = "";
				file_name           = "";
				folder_name         = "";
				folder_path         = "";
				folder_total        = 0;
				folder_depth        = 0;
				create_status       = VS_OK;
				root_folder         = NULL;
				#ifdef OLD_TIMELINE
				first_minute_folder = NULL;
				last_minute_folder  = NULL;
				#endif
			}

	};


	// ---------- Timeline Class ----------
	class Timeline
	{

	public:

		// ---------- ctot ----------
		/*!
		\brief Constructor
		\author Gareth Edwards
		*/
		Timeline::Timeline()
		{
			callback_on_file = NULL;
			callback_on_folder = NULL;
		}


		// ---------- dtor ----------
		/*!
		\brief Constructor
		\author Gareth Edwards
		*/
		Timeline::~Timeline()
		{
			;
		}

	private:

		// ---------- properties ----------

		vs_system::TimelineInfo timeline_info;

		INT(*callback_on_file)(vs_system::TimelineInfo *);
		INT(*callback_on_folder)(vs_system::TimelineInfo *);


	public:


		// ---------- methods ----------


		// ---------- Create ----------
		/*!
		\brief Create a timeline vnode dataset
		\author Gareth Edwards
		*/
		INT Create(
				CHAR *archive_path,
				BOOL  refresh,
				BOOL  report
			)
		{

			// ---- prep path
				CHAR tmp[VS_MAXCHARPATHLEN];
				strcpy(tmp, archive_path);
				int len = strlen(tmp);
				for (int i = 0; i < len; i++)
					tmp[i] = tmp[i] == '\\' ? '/' : tmp[i];
				strcat(tmp, "/archive/");
				timeline_info.archive_path.assign(tmp);

			// ---- local
				INT result = 0;
				HANDLE hFind = INVALID_HANDLE_VALUE;
				WIN32_FIND_DATA fdata;
				std:: string fullpath;
				std::stack<std::string> folders;
				std::stack<INT> depth;

			// ---- push folder path
				folders.push(tmp);

			// ---- push folder depth
				INT current_folder_depth = 0;
				depth.push(current_folder_depth);

			// ---- init folder counter
				timeline_info.folder_total = 0;


			// ---- recurse until no more folders
				while ( !folders.empty() )
				{

					// ---- init top
						timeline_info.path = folders.top();
						current_folder_depth = depth.top();

					// ---- add to node data
						result = AddFolder(refresh);

					// ---- FindFirstFile path
						fullpath = timeline_info.path + "\\" + timeline_info.mask;

					// ---- pop
						folders.pop();
						depth.pop();

					// ---- list this folder
						hFind = FindFirstFile(fullpath.c_str(), &fdata);
						if ( hFind != INVALID_HANDLE_VALUE )
						{
								do
								{
										if ( strcmp(fdata.cFileName, ".") != 0 &&
											strcmp(fdata.cFileName, "..") != 0)
										{
			
												if ( fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
												{

													// ---- push
														folders.push(timeline_info.path + "\\" + fdata.cFileName);
														depth.push(current_folder_depth + 1);

													// ----  info
														timeline_info.folder_total++;
														if ( callback_on_folder != NULL )
														{
															timeline_info.file_name    = "";
															timeline_info.folder_path  = folders.top();
															timeline_info.folder_name  = fdata.cFileName;
															timeline_info.folder_depth = depth.top() - 1;
															INT result = callback_on_folder(&timeline_info);
														}

												}

												else break;
										}
								}
								while ( FindNextFile(hFind, &fdata ) != 0 );
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


			// ---- "cross" link folder nodes
				VNode *link_stack[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
				UINT link_index = 0;
				result = CrossLinkFolders(
						timeline_info.root_folder,
						link_index,
						link_stack
					);


			//
			// note: if archive poorly formed then cross linking may not be 
			//       possible - null nodes are detected below and the
			//       create_status reset VS_ERROR
			//
				timeline_info.create_status = VS_OK;


			// ---- init "first" stack
				timeline_info.datetime[TimelineInfo::DateTime::YEAR] =
					timeline_info.root_folder->GetFirst();
				for (INT i = 1; i <= 4; i++)
				{
					if ( timeline_info.datetime[i - 1] != NULL )
						timeline_info.datetime[i] =
							timeline_info.datetime[i - 1]->GetFirst();
					else
						timeline_info.create_status = VS_ERROR;
				}


			// ---- report ? -- "cross" linking
				if ( report )
				{
					for (INT i = 0; i <= 4; i++)
						ReportCrossLinkFolders(timeline_info.datetime[i]);
				}


			// ---- set last folder
				#ifdef OLD_TIMELINE
				VNode *last = timeline_info.root_folder;
				while (last != NULL)
				{
					timeline_info.last_minute_folder = last;
					last = last->GetLast();
				}
				#endif

			// ---- zap info
				timeline_info.path         = "";
				timeline_info.file_name    = "";
				timeline_info.folder_path  = "";
				timeline_info.folder_name  = "";
				timeline_info.folder_depth = 0;

			return timeline_info.folder_total;
		}


		// ---------- GetCreateStatus----------
		/*!
		\brief return create status
		\author Gareth Edwards
		\rfeturn VS_ERROR or VS_OK
		*/
		INT GetCreateStatus() { return timeline_info.create_status;  }


		// ---------- Scan_01 ----------
		/*!
		\brief Scan timeline folders 
		\author Gareth Edwards
		*/
		INT Scan_01()
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

			// ---- initialise folder stack
				strcpy(folder_stack[++folder_stack_top], timeline_info.archive_path.c_str() );


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

											// ---- files ?
												// -- listed AFTER folders - so exit loop
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


		// ---------- Scan ----------
		/*!
		\brief Scan timeline folders
		\author Gareth Edwards
		*/
		INT Scan()
		{

			// ---- local
				INT result = 0;

			// ---- local timeline
				std::string tf_path;
				tf_path.assign(timeline_info.archive_path);

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
					fullpath = tf_path + "\\" + timeline_info.mask;

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

											// ---- folder ?
												// push current path onto foldersa stack
												if ( fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
												{
													folder_counter++;
													folders.push(tf_path + "\\" + fdata.cFileName);
												}
											// ---- files ?
												// -- listed AFTER folders - so exit loop
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


		// ---------- AddFolder ----------
		/*!
		\brief Add folder to timeline
		\author Gareth Edwards
		*/
		INT AddFolder(BOOL refresh)
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
				strcpy(temp, timeline_info.path.c_str());
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
								timeline_info.root_folder = new VNode(stack[num_words - 1]);
								timeline_info.folder_total++;
						}
				}

			// ELSE --
				// add allocate and create folder nodes
				else
				{
						// ---- FOR each hierarchical level on "word" stack except first...
						VNode *parent = timeline_info.root_folder;
						for ( WORD w=1; w<num_words; w++ )
						{
							// ---- search for matching child name
							VNode *child = parent->GetFirst();
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
									VNode* new_node = NULL;
									if ( refresh )
									{
										new_node = new VNode(stack[w], parent, 1);
									}
									else
									{
										new_node = new VNode(stack[w], parent);
									}
									return VS_OK;
								}

							// ---- next hierarchical level on "word" stack
								parent = child;

						}

				}

			return VS_OK;
		}


		// ---------- CrossLinkFolders ----------
		/*!
		\brief Recursively cross link folders
		\author Gareth Edwards
		*/
		INT CrossLinkFolders(
				VNode *node,
				UINT   link_index,
				VNode *link[]
			)
		{

			// ---- ? leaf -- set "first_minute_folder"
				#ifdef OLD_TIMELINE
				if ( node->GetFirst() == NULL )
					if ( timeline_info.first_minute_folder == NULL )
						 timeline_info.first_minute_folder = node;
				#endif


			// ---- ? previous -- set previous "cross" link to "this" node
				VNode *previous = link[link_index];
				if ( previous != NULL )
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
				VNode *child = node->GetFirst();
				while ( child )
				{
					INT result = CrossLinkFolders(child, link_index+1, link);
					child = child->GetNext();
				}

			return VS_OK;
		}


		// ---------- ReportAllFolders ----------
		/*!
		\brief report all folders
		\author Gareth Edwards
		\return INT VS_ERROR or VS_OK
		*/
		INT ReportAllFolders(VNode *parent)
		{

			VNode *grandparent = parent->GetParent();
			if ( grandparent )
				std::cout << grandparent->GetName() << " <- " << parent->GetName() << std::endl;
			else
				std::cout << parent->GetName() << std::endl;
			VNode *child = parent->GetFirst();
			while ( child )
			{
				INT result = ReportAllFolders(child);
				child = child->GetNext();
			}

			return VS_OK;
		}


		// ---------- ReportCrossLinkFolders ----------
		/*!
		\brief report all folders
		\author Gareth Edwards
		\return INT VS_ERROR or VS_OK
		*/
		INT ReportCrossLinkFolders(VNode *node)
		{

			if ( node == NULL ) return VS_ERROR;

			if ( node->GetParent() != NULL ) std::cout << node->GetParent()->GetName() << " : ";

			while ( node )
			{
				VNode *link_p = node->GetLinkP();
				VNode *link_s = node->GetLinkS();

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


		// ---------- Process ----------
		/*!
		\brief process cross link folders
		\author Gareth Edwards
		\return INT VS_ERROR or VS_OK
		*/
		INT Process(
				TimelineInfo::DateTime dt
			)
		{
			
			// ---- error ?
			if ( dt < TimelineInfo::DateTime::YEAR ||
					dt > TimelineInfo::DateTime::MINUTE)
						return VS_ERROR;


			// --- walk across ----
				VNode *node = timeline_info.datetime[dt];
				while ( node )
				{
					if ( callback_on_folder != NULL )
					{
						VNode *vn_p = node->GetParent();
						std::string paf = node->GetName();

						int start_dt = dt - 1 < 0 ? 0 : dt - 1;
						for (int i = start_dt; i >= 0; i--)
						{
							paf  = "/" + paf;
							paf  = vn_p->GetName() + paf;
							vn_p = vn_p->GetParent();
						}

						timeline_info.file_name    = "";
						timeline_info.folder_path  = paf;
						timeline_info.folder_name  = node->GetName();
						timeline_info.folder_depth = dt;
						INT result = callback_on_folder(&timeline_info);
					}
					node = node->GetLinkS();
				}


			return VS_OK;
		}


		// ---------- Clear ----------
		/*!
		\brief Clear
		\author Gareth Edwards
		\return INT total - # of nodes deleted
		\note including root node!
		*/
		INT Clear()
		{

			// ---- using S property ----


			// ---- link all nodes
				VNode *last = timeline_info.root_folder;
				ClearLink(timeline_info.root_folder, &last);
				

			// --- delete all nodes
				VNode* node = timeline_info.root_folder;
				int total = 0;
				while (node)
				{
					VNode *next = node->GetLinkS();
					delete node;
					total++;
					node = next;
				}

			// ---- re-init
				timeline_info.Initialise();

			return total;
		}


		// ---------- ClearLink ----------
		/*!
		\brief link all nodes using S property
		\author Gareth Edwards
		\return INT VS_ERROR or VS_OK
		*/
		INT ClearLink(VNode *node, VNode **last)
		{

			// ---- !root ? -- link last to this
			VNode *l = *last;
			if ( node != timeline_info.root_folder )
					l->SetLinkS(node);

			// ---- last IS this
				*last = node;

			// ---- repeat
				VNode *child = node->GetFirst();
				while ( child )
				{
					INT result = ClearLink(child, last);
					child = child->GetNext();
				}

			return VS_OK;
		}


		// ---------- SetCallbackOnFile ----------
		/*!
		\brief SetCallbackOnFile
		\author Gareth Edwards
		\return INT - ERROR <=0 < VS_OK
		*/
		INT SetCallbackOnFile(
				INT(*callback)(vs_system::TimelineInfo *info)
			)
		{
			callback_on_file = callback;
			return VS_OK;
		}


		// ---------- SetCallbackOnFolder ----------
		/*!
		\brief SetCallbackOnFolder
		\author Gareth Edwards
		\return INT - ERROR <=0 < VS_OK
		*/
		INT SetCallbackOnFolder(
				INT(*callback)(vs_system::TimelineInfo *info)
			)
		{
			callback_on_folder = callback;
			return VS_OK;
		}


	};


}


///////////////////////////////////////////////////////////////////////////////
