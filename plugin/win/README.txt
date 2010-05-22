/**************************************************************\
	  CoNetServ - HOWTO compile in MSVS
\**************************************************************/
Required:
1.Microsoft Visual Studio 2008 sp1
( without sp1 there is a problem with linking )

2.Download Gecko SDK
http://developer.mozilla.org/en/docs/Gecko_SDK

Build:
1.Open npconetserv\npconetserv.vcproj in MVSVS
2.extract Gecko SDK on your disc
3.set project|properties|c/c++/Aditional Include Directories 
to point to xulrunner-sdk/sdk/include 

