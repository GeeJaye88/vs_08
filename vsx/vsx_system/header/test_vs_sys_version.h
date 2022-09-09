////////////////////////////////////////////////////////////////////////////////

// ---------- vs_sys_version.h ----------
/*!
\file vs_sys_version.h
\brief VS System [Major].[Minor].[Patch].[Build] version
\author Gareth Edwards
*/


// ---------- VS System [Major] .[Minor].[Patch].[Build]version ----------
//
// Note: Based on Semantic Versioning 2.0.0
// http://semver.org/
//
// Summary
//
// Given a version number MAJOR.MINOR.PATCH, increment the:
//
// MAJOR version when you make incompatible API changes,
// MINOR version when you add functionality in a backwards - compatible manner, and
// PATCH version when you make backwards - compatible bug fixes.
//
// BUILD (metadata)SHOULD b e ignored when determining version precedence
//


// ---------- VS_VERSION_INFO VERSIONINFO  ----------
//
// See MS VS_VERSION_INFO resource file info
//


// ---------- VS_SYS_VERSION ----------

#define VS_SYS_VERSION_MAJOR 6 // 2019/1/2
#define VS_SYS_VERSION_MINOR 1 // 2019/1/2
#define VS_SYS_VERSION_PATCH 1 // 2019/1/2


// Note: see "FILEVERSION [#,#,#,#]" and VALUE "FileVersion", "[#,#,#,#]", e.g. "4,1,1,1441"
#define VS_SYS_VERSION_BUILD 1 // 2019/1/2


// Note: see VALUE "CompanyName",  "[description]", e.g. "Observant Innovations"
#define VS_SYS_COMPANY_NAME "Observant Innovations"


// Note: see VALUE "LegalCopyright",  "[Copyright © #]", e.g. "2013 - 2018"
#define VS_SYS_COPYRIGHT_YEAR "2013 - 2019"































