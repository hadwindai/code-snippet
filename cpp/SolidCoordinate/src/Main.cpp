
#include <uf.h>
#include <uf_ui.h>
#include <iostream>
#include <NXOpen/Session.hxx>
#include <NXOpen/NXException.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/UI.hxx>
#include <NXOpen/UIStyler_Styler.hxx>
#include "SolidCoordinateDialog.hxx"

static void PrintErrorMessage(int errorCode);

extern DllExport void ufusr(char *parm, int *returnCode, int rlen){
	int errorCode = UF_initialize();
	if(errorCode == 0){
		try{				
			NXOpen::Session *session = NXOpen::Session::GetSession();
			NXOpen::Part *part = session->Parts()->Work();
			if(part == NULL){
				part = session->Parts()->NewDisplay("c:\\sc.prt", NXOpen::Part::UnitsMillimeters);		
			}			
			HADWIN::SolidCoordinateDialog *scd = new HADWIN::SolidCoordinateDialog(part);			
			scd->show();
			scd->dispose();
			delete scd;
			scd = NULL;
		}catch(NXOpen::NXException &ex){	
			NXOpen::UI::GetUI()->NXMessageBox()->Show("UI Styler", NXOpen::NXMessageBox::DialogTypeError, ex.Message());
			std::cerr << ex.GetMessage() << std::endl;
		}
		errorCode = UF_terminate();
	}
	PrintErrorMessage(errorCode);
}

extern int ufusr_ask_unload(){
	return (NXOpen::Session::LibraryUnloadOptionImmediately);
}

static void PrintErrorMessage(int errorCode){
	if ( 0 != errorCode ){       
		char message[133];
		UF_get_fail_message(errorCode, message);      
		UF_UI_set_status(message);      
		std::cerr << std::endl << "Error:" << std::endl << message << std::ends;      
	}
}
