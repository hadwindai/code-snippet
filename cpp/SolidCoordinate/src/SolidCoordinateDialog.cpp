
#include <NXOpen/UI.hxx>
#include <NXOpen/UIStyler_Styler.hxx>
#include "SolidCoordinateDialog.hxx"
#include "FeatureSolidCoordinate.hxx"
#include "ExpressionSolidCoordinate.hxx"
#include "DrawingSolidCoordinate.hxx"

namespace HADWIN{

SolidCoordinateDialog::SolidCoordinateDialog(NXOpen::Part *part):isDispose(false),method(0){
	try {
		solidCoordinate = new FeatureSolidCoordinate(part);
		dialog = NXOpen::UI::GetUI()->Styler()->CreateStylerDialog("SolidCoordinate.dlg");
		initializeUIStylerDialog();
	}catch (const NXOpen::NXException& ex){
		showErrorDialog(ex.Message());
	}
}

SolidCoordinateDialog::~SolidCoordinateDialog(){
	try{
		if (!isDispose){
			delete scDialog;
			delete scCreateMethod;
			delete scRealDiameter;
			delete scScaleDiameter;
			delete scSynchronize;
			delete scRealXlength;
			delete scScaleXlength;
			delete scRealYlength;
			delete scScaleYlength;
			delete scRealZlength;
			delete scScaleZlength;
			delete dialog;
		}
		isDispose = true;
	}catch (const NXOpen::NXException& ex){
		showErrorDialog(ex.Message());
	}		
}

void SolidCoordinateDialog::initializeUIStylerDialog(){
	try{
		scDialog = dialog->GetDialogIndex("UF_STYLER_DIALOG_INDEX");
		scDialog->AddConstructHandler(make_callback(this, &SolidCoordinateDialog::constructCallback), false);
		scDialog->AddOkayHandler(make_callback(this, &SolidCoordinateDialog::okCallback), false);
		scDialog->AddApplyHandler(make_callback(this, &SolidCoordinateDialog::applyCallback), false);
		scDialog->AddCancelHandler(make_callback(this, &SolidCoordinateDialog::cancelCallback), false);
		scCreateMethod = dialog->GetToolPalette("CREATE_METHOD");
		scCreateMethod->AddActivateHandler(make_callback(this, &SolidCoordinateDialog::createMethodCallback), false);
		scRealDiameter = dialog->GetRealItem("REAL_DIAMETER");
		scRealDiameter->AddActivateHandler(make_callback(this, &SolidCoordinateDialog::realDiameterCallback), false);
		scScaleDiameter = dialog->GetRealScale("SCALE_DIAMETER");
		scScaleDiameter->AddValueChangedHandler(make_callback(this, &SolidCoordinateDialog::scaleDiameterCallback), false);
		scScaleDiameter->AddDragHandler(make_callback(this, &SolidCoordinateDialog::scaleDiameterCallback), false);
		scSynchronize = dialog->GetToggle("SYNCHRONIZE");
		scSynchronize->AddValueChangedHandler(make_callback(this, &SolidCoordinateDialog::synchronizeCallback), false);
		scRealXlength = dialog->GetRealItem("REAL_XLENGTH");
		scRealXlength->AddActivateHandler(make_callback(this, &SolidCoordinateDialog::realXLengthCallback), false);
		scScaleXlength = dialog->GetRealScale("SCALE_XLENGTH");
		scScaleXlength->AddValueChangedHandler(make_callback(this, &SolidCoordinateDialog::scaleXLengthCallback), false);
		scScaleXlength->AddDragHandler(make_callback(this, &SolidCoordinateDialog::scaleXLengthCallback), false);
		scRealYlength = dialog->GetRealItem("REAL_YLENGTH");
		scRealYlength->AddActivateHandler(make_callback(this, &SolidCoordinateDialog::realYLengthCallback), false);
		scScaleYlength = dialog->GetRealScale("SCALE_YLENGTH");
		scScaleYlength->AddValueChangedHandler(make_callback(this, &SolidCoordinateDialog::scaleYLengthCallback), false);
		scScaleYlength->AddDragHandler(make_callback(this, &SolidCoordinateDialog::scaleYLengthCallback), false);
		scRealZlength = dialog->GetRealItem("REAL_ZLENGTH");
		scRealZlength->AddActivateHandler(make_callback(this, &SolidCoordinateDialog::realZLengthCallback), false);
		scScaleZlength = dialog->GetRealScale("SCALE_ZLENGTH");
		scScaleZlength->AddValueChangedHandler(make_callback(this, &SolidCoordinateDialog::scaleZLengthCallback), false);
		scScaleZlength->AddDragHandler(make_callback(this, &SolidCoordinateDialog::scaleZLengthCallback), false);		
	}catch (const NXOpen::NXException& ex){
		showErrorDialog(ex.Message());
	}	
}	

NXOpen::UIStyler::DialogResponse SolidCoordinateDialog::show(){
	NXOpen::UIStyler::DialogResponse retValue;
	try{
		solidCoordinate->create();
		retValue = dialog->Show();
	}catch(const NXOpen::NXException& ex){
		showErrorDialog(ex.Message());
	}
	return retValue;
}

void SolidCoordinateDialog::dispose(){
	try{
		if (!isDispose){
			delete scDialog;
			delete scCreateMethod;
			delete scRealDiameter;
			delete scScaleDiameter;
			delete scSynchronize;
			delete scRealXlength;
			delete scScaleXlength;
			delete scRealYlength;
			delete scScaleYlength;
			delete scRealZlength;
			delete scScaleZlength;
			delete dialog;
		}
		isDispose = true;
	}catch (const NXOpen::NXException& ex){
		showErrorDialog(ex.Message());
	}
}

NXOpen::UIStyler::DialogState SolidCoordinateDialog::constructCallback(NXOpen::UIStyler::StylerEvent* eventObject){
	try{
		scDialog->SetTitle("3D Solid Coordinate - Detosoft");
	}catch (const NXOpen::NXException& ex){
		showErrorDialog(ex.Message());
	}
	return NXOpen::UIStyler::DialogStateContinueDialog;
}

NXOpen::UIStyler::DialogState SolidCoordinateDialog::okCallback(NXOpen::UIStyler::StylerEvent* eventObject){
	try{
		if(!checkValues()){
			return NXOpen::UIStyler::DialogStateContinueDialog;
		}
		solidCoordinate->commit();
	}catch (const NXOpen::NXException& ex){
		showErrorDialog(ex.Message());
	}
	return NXOpen::UIStyler::DialogStateExitDialog;
}

NXOpen::UIStyler::DialogState SolidCoordinateDialog::applyCallback(NXOpen::UIStyler::StylerEvent* eventObject){
	try{
		if(!checkValues()){
			return NXOpen::UIStyler::DialogStateContinueDialog;
		}
		solidCoordinate->commit();
	}catch (const NXOpen::NXException& ex){
		showErrorDialog(ex.Message());
	}
	return NXOpen::UIStyler::DialogStateExitDialog;
}

NXOpen::UIStyler::DialogState SolidCoordinateDialog::cancelCallback(NXOpen::UIStyler::StylerEvent* eventObject){
	try{			
		solidCoordinate->remove();
		delete solidCoordinate;
	}catch (const NXOpen::NXException& ex){
		showErrorDialog(ex.Message());
	}
	return NXOpen::UIStyler::DialogStateExitDialog;
}

NXOpen::UIStyler::DialogState SolidCoordinateDialog::createMethodCallback(NXOpen::UIStyler::StylerEvent* eventObject){
	try{
		int value = scCreateMethod->ItemValue();
		if(method != value){
			method = value;
			changeSolidCoordinateInstance(value);
		}			
	}catch (const NXOpen::NXException& ex){		
		showErrorDialog(ex.Message());
	}
	return NXOpen::UIStyler::DialogStateContinueDialog;	
}

NXOpen::UIStyler::DialogState SolidCoordinateDialog::realDiameterCallback(NXOpen::UIStyler::StylerEvent* eventObject){
	double value = 0.0;
	try{			
		value = scRealDiameter->ItemValue();
		if(value <= 0.0){
			showErrorDialog("The value of axis diameter can't be less than zero!");
		}else if(solidCoordinate->getDiameter() != value){
			solidCoordinate->updateDiameter(value);
			scScaleDiameter->SetItemValue(value);
		}
	}catch (const NXOpen::NXException& ex){
		showErrorDialog(ex.Message());
	}
	return NXOpen::UIStyler::DialogStateContinueDialog;
}

NXOpen::UIStyler::DialogState SolidCoordinateDialog::scaleDiameterCallback(NXOpen::UIStyler::StylerEvent* eventObject){
	double value = 0.0;
	try{			
		value = scScaleDiameter->ItemValue();
		if(value <= 0.00){
			showErrorDialog("The value of axis diameter can't be less than zero!");
		}else if(solidCoordinate->getDiameter() != value){
			solidCoordinate->updateDiameter(value);
			scRealDiameter->SetItemValue(value);
		}
	}catch (const NXOpen::NXException& ex){
		showErrorDialog(ex.Message());
	}
	return NXOpen::UIStyler::DialogStateContinueDialog;
}

NXOpen::UIStyler::DialogState SolidCoordinateDialog::synchronizeCallback(NXOpen::UIStyler::StylerEvent* eventObject){
	double value = solidCoordinate->getXHeight();
	try{
		if((scSynchronize->ItemValue()) && (value!=solidCoordinate->getYHeight()) && (value!=solidCoordinate->getZHeight())){	
			synchronizeHeight(value);
		}
	}catch (const NXOpen::NXException& ex){
		showErrorDialog(ex.Message());
	}
	return NXOpen::UIStyler::DialogStateContinueDialog;
}

NXOpen::UIStyler::DialogState SolidCoordinateDialog::realXLengthCallback(NXOpen::UIStyler::StylerEvent* eventObject){
	double value = 0.0;
	try{
		value = scRealXlength->ItemValue();			
		if(value <= 0.0){
			showErrorDialog("The value of X length can't be less than zero!");
		}else if(solidCoordinate->getXHeight() != value){
			if(scSynchronize->ItemValue()){
				synchronizeHeight(value);
			}else{
				solidCoordinate->updateXHeight(value);
				scScaleXlength->SetItemValue(value);
			}
		}
	}catch (const NXOpen::NXException& ex){			
		showErrorDialog(ex.Message());
	}	
	return NXOpen::UIStyler::DialogStateContinueDialog;	
}

NXOpen::UIStyler::DialogState SolidCoordinateDialog::scaleXLengthCallback(NXOpen::UIStyler::StylerEvent* eventObject){
	double value = 0.0;
	try{
		value = scScaleXlength->ItemValue();
		if(value <= 0.0){
			showErrorDialog("The value of X length can't be less than zero!");				
		}else if(solidCoordinate->getXHeight() != value){
			if(scSynchronize->ItemValue()){	
				synchronizeHeight(value);				
			}else{
				solidCoordinate->updateXHeight(value);
				scRealXlength ->SetItemValue(value);
			}
		}
	}catch (const NXOpen::NXException& ex){		
		showErrorDialog(ex.Message());
	}
	return NXOpen::UIStyler::DialogStateContinueDialog;
}

NXOpen::UIStyler::DialogState SolidCoordinateDialog::realYLengthCallback(NXOpen::UIStyler::StylerEvent* eventObject){
	double value = 0.0;
	try{
		value = scRealYlength->ItemValue();
		if(value <= 0.0){
			showErrorDialog("The value of Y length can't be less than zero!");				
		}else if(solidCoordinate->getYHeight() != value){
			if(scSynchronize->ItemValue()){	
				synchronizeHeight(value);				
			}else{
				solidCoordinate->updateYHeight(value);
				scScaleYlength->SetItemValue(value);
			}
		}
	}catch (const NXOpen::NXException& ex){			
		showErrorDialog(ex.Message());
	}
	return NXOpen::UIStyler::DialogStateContinueDialog;		
}

NXOpen::UIStyler::DialogState SolidCoordinateDialog::scaleYLengthCallback(NXOpen::UIStyler::StylerEvent* eventObject){
	double value = 0.0;
	try{
		value = scScaleYlength->ItemValue();
		if(value <= 0.0){
			showErrorDialog("The value of Y length can't be less than zero!");				
		}else if(solidCoordinate->getYHeight() != value){
			if(scSynchronize->ItemValue()){	
				synchronizeHeight(value);				
			}else{
				solidCoordinate->updateYHeight(value);
				scRealYlength->SetItemValue(value);
			}
		}
	}catch (const NXOpen::NXException& ex){			
		showErrorDialog(ex.Message());
	}
	return NXOpen::UIStyler::DialogStateContinueDialog;
}

NXOpen::UIStyler::DialogState SolidCoordinateDialog::realZLengthCallback(NXOpen::UIStyler::StylerEvent* eventObject){
	double value = 0.0;
	try{	
		value = scRealZlength->ItemValue();
		if(value <= 0.0){
			showErrorDialog("The value of Z length can't be less than zero!");
		}else if(solidCoordinate->getZHeight() != value){
			if(scSynchronize->ItemValue()){
				synchronizeHeight(value);
			}else{
				solidCoordinate->updateZHeight(value);
				scScaleZlength->SetItemValue(value);
			}
		}
	}catch(const NXOpen::NXException& ex){
		showErrorDialog(ex.Message());
	}		
	return NXOpen::UIStyler::DialogStateContinueDialog;
}

NXOpen::UIStyler::DialogState SolidCoordinateDialog::scaleZLengthCallback(NXOpen::UIStyler::StylerEvent* eventObject){
	double value = 0.0;
	try{
		value = scScaleZlength->ItemValue();
		if(value <= 0.0){
			showErrorDialog("The value of Z length can't be less than zero!");
		}else if(solidCoordinate->getZHeight() != value){
			if(scSynchronize->ItemValue()){
				synchronizeHeight(value);
			}else{
				solidCoordinate->updateZHeight(value);
				scRealZlength->SetItemValue(value);
			}
		}
	}catch (const NXOpen::NXException& ex){
		showErrorDialog(ex.Message());
	}		
	return NXOpen::UIStyler::DialogStateContinueDialog;
}

bool SolidCoordinateDialog::checkValues(){
	if((scRealDiameter->ItemValue()) <= 0){
		showErrorDialog("The value of axis diameter can't be less than zero!");
		return false;
	}
	if((scRealXlength->ItemValue()) <= 0){
		showErrorDialog("The value of X length can't be less than zero!");
		return false;
	}
	if((scRealYlength->ItemValue()) <= 0){
		showErrorDialog("The value of Y length can't be less than zero!");
		return false;
	}
	if((scRealZlength->ItemValue()) <= 0){
		showErrorDialog("The value of Z length can't be less than zero!");
		return false;
	}
	return true;
}

void SolidCoordinateDialog::synchronizeHeight(const double value){
	solidCoordinate ->updateHeight(value);
	scRealXlength->SetItemValue(value);
	scScaleXlength->SetItemValue(value);
	scRealYlength->SetItemValue(value);
	scScaleYlength->SetItemValue(value);
	scRealZlength->SetItemValue(value);
	scScaleZlength->SetItemValue(value);
}

void SolidCoordinateDialog::changeSolidCoordinateInstance(const int value){
	SolidCoordinate *newSolidCoordinate;	
	solidCoordinate->remove();
	switch(value){
	case 0:
		newSolidCoordinate = new FeatureSolidCoordinate(solidCoordinate->getPart(), solidCoordinate->getDiameter(), solidCoordinate->getXHeight(), solidCoordinate->getYHeight(), solidCoordinate->getZHeight());
		break;
	case 1:
		newSolidCoordinate = new ExpressionSolidCoordinate(solidCoordinate->getPart(), solidCoordinate->getDiameter(), solidCoordinate->getXHeight(), solidCoordinate->getYHeight(), solidCoordinate->getZHeight());
		break;
	case 2:
		newSolidCoordinate = new DrawingSolidCoordinate(solidCoordinate->getPart(), solidCoordinate->getDiameter(), solidCoordinate->getXHeight(), solidCoordinate->getYHeight(), solidCoordinate->getZHeight());
		break;
	}
	delete solidCoordinate;
	solidCoordinate = newSolidCoordinate;
	solidCoordinate->create();
}
}
