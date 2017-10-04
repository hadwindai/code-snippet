
#ifndef SOLID_COORDINATE_DIALOG_HXX_INCLUDED
#define SOLID_COORDINATE_DIALOG_HXX_INCLUDED

#include <uf_defs.h>
#include <iostream>
#include <NXOpen/Session.hxx>
#include <NXOpen/UI.hxx>
#include <NXOpen/Callback.hxx>
#include <NXOpen/NXException.hxx>
#include <NXOpen/UIStyler_Dialog.hxx>
#include <NXOpen/UIStyler_StylerEvent.hxx>
#include <NXOpen/NXMessageBox.hxx>
#include <NXOpen/UIStyler_CollapsibleGroup.hxx>
#include <NXOpen/UIStyler_DialogItem.hxx>
#include <NXOpen/UIStyler_GroupBox.hxx>
#include <NXOpen/UIStyler_LabelItem.hxx>
#include <NXOpen/UIStyler_RealItem.hxx>
#include <NXOpen/UIStyler_RealScale.hxx>
#include <NXOpen/UIStyler_Separator.hxx>
#include <NXOpen/UIStyler_Toggle.hxx>
#include <NXOpen/UIStyler_ToolPalette.hxx>

#include "SolidCoordinate.hxx"
#include "FeatureSolidCoordinate.hxx"


namespace HADWIN{

class SolidCoordinateDialog{

public:
	SolidCoordinateDialog(NXOpen::Part *part);
	~SolidCoordinateDialog();
	void dispose();
	NXOpen::UIStyler::DialogResponse show();
	void registerWithUiMenu();

public:
	NXOpen::UIStyler::DialogState applyCallback(NXOpen::UIStyler::StylerEvent* eventObject);
	NXOpen::UIStyler::DialogState cancelCallback(NXOpen::UIStyler::StylerEvent* eventObject);
	NXOpen::UIStyler::DialogState constructCallback(NXOpen::UIStyler::StylerEvent* eventObject);
	NXOpen::UIStyler::DialogState createMethodCallback(NXOpen::UIStyler::StylerEvent* eventObject);
	NXOpen::UIStyler::DialogState okCallback(NXOpen::UIStyler::StylerEvent* eventObject);
	NXOpen::UIStyler::DialogState realDiameterCallback(NXOpen::UIStyler::StylerEvent* eventObject);
	NXOpen::UIStyler::DialogState realXLengthCallback(NXOpen::UIStyler::StylerEvent* eventObject);
	NXOpen::UIStyler::DialogState realYLengthCallback(NXOpen::UIStyler::StylerEvent* eventObject);
	NXOpen::UIStyler::DialogState realZLengthCallback(NXOpen::UIStyler::StylerEvent* eventObject);
	NXOpen::UIStyler::DialogState scaleDiameterCallback(NXOpen::UIStyler::StylerEvent* eventObject);
	NXOpen::UIStyler::DialogState scaleXLengthCallback(NXOpen::UIStyler::StylerEvent* eventObject);
	NXOpen::UIStyler::DialogState scaleYLengthCallback(NXOpen::UIStyler::StylerEvent* eventObject);
	NXOpen::UIStyler::DialogState scaleZLengthCallback(NXOpen::UIStyler::StylerEvent* eventObject);
	NXOpen::UIStyler::DialogState synchronizeCallback(NXOpen::UIStyler::StylerEvent* eventObject);

private:
	void initializeUIStylerDialog();
	bool checkValues();
	void synchronizeHeight(const double value);
	void changeSolidCoordinateInstance(const int value);
	void showErrorDialog(const char *message){
		NXOpen::UI::GetUI()->NXMessageBox()->Show("Error Message", NXOpen::NXMessageBox::DialogTypeError, message);
	}

private:
	SolidCoordinate *solidCoordinate;
	bool isDispose;
	int method;
	NXOpen::UIStyler::Dialog *dialog;
	NXOpen::UIStyler::DialogItem *scDialog;
	NXOpen::UIStyler::ToolPalette *scCreateMethod;
	NXOpen::UIStyler::RealItem *scRealDiameter;
	NXOpen::UIStyler::RealScale *scScaleDiameter;
	NXOpen::UIStyler::Toggle *scSynchronize;
	NXOpen::UIStyler::RealItem *scRealXlength;
	NXOpen::UIStyler::RealScale *scScaleXlength;
	NXOpen::UIStyler::RealItem *scRealYlength;
	NXOpen::UIStyler::RealScale *scScaleYlength;
	NXOpen::UIStyler::RealItem *scRealZlength;
	NXOpen::UIStyler::RealScale *scScaleZlength;
};
}

#endif
