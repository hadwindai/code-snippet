

#include <NXOpen/ImportManager.hxx>
#include <NXOpen/Importer.hxx>
#include <NXOpen/PartImporter.hxx>
#include <NXOpen/NXMatrix.hxx>
#include <NXOpen/NXMatrixCollection.hxx>
#include <NXOpen/Expression.hxx>
#include <NXOpen/ExpressionCollection.hxx>
#include <NXOpen/Features_Feature.hxx>
#include "ExpressionSolidCoordinate.hxx"

namespace HADWIN{
		
	const std::string ExpressionSolidCoordinate::filename("SolidCoordinate.prt");
	const std::string ExpressionSolidCoordinate::diameterComment("Solid Coordinate Origin Diameter");
	const std::string ExpressionSolidCoordinate::xHeightComment("Solid Coordinate X Axis Height");
	const std::string ExpressionSolidCoordinate::yHeightComment("Solid Coordinate Y Axis Height");
	const std::string ExpressionSolidCoordinate::zHeightComment("Solid Coordinate Z Axis Height");

	ExpressionSolidCoordinate::ExpressionSolidCoordinate(NXOpen::Part *_part):
		SolidCoordinate(_part)		
	{
	}

	ExpressionSolidCoordinate::ExpressionSolidCoordinate(NXOpen::Part *_part, double _diameter, double _height):
		SolidCoordinate(_part, _diameter, _height)
	{			
	}

	ExpressionSolidCoordinate::ExpressionSolidCoordinate(NXOpen::Part *_part, double _diameter, double _xHeight, double _yHeight, double _zHeight):
		SolidCoordinate(_part, _diameter, _xHeight, _yHeight, _zHeight)
	{		
	}	

	ExpressionSolidCoordinate::~ExpressionSolidCoordinate()	{	
		delete diameterExpression;
		delete xHeightExpression;
		delete yHeightExpression;
		delete zHeightExpression;	
	}

	void ExpressionSolidCoordinate::create(){		
		cleanup();
		import();
		bindExpression();
		initializeValue();
	}	

	void ExpressionSolidCoordinate::commit(){
		printf("Commit the Expression Solid Coordinate!");		
	}

	void ExpressionSolidCoordinate::remove(){
		int i = 0;		
		tag_t *features;
		uf_list_p_t list;
		UF_MODL_ask_features_of_exp(diameterExpression->Tag(), &i, &features);
		UF_MODL_create_list(&list);
		for(int j=0; j < i; j++){			
			UF_MODL_put_list_item(list, features[j]);			
		}
		UF_free(features);
		UF_MODL_delete_feature(list);
		UF_MODL_delete_list(&list);		
	}

	void ExpressionSolidCoordinate::fireDiameter(){
		editExpression(diameterExpression, diameter);
		updateExpression();	
	}

	void ExpressionSolidCoordinate::fireHeight(){
		editExpression(xHeightExpression, xHeight);
		editExpression(yHeightExpression, yHeight);
		editExpression(zHeightExpression, zHeight);
		updateExpression();
	}

	void ExpressionSolidCoordinate::fireXHeight(){
		editExpression(xHeightExpression, xHeight);
		updateExpression();	
	}	

	void ExpressionSolidCoordinate::fireYHeight(){
		editExpression(yHeightExpression, yHeight);
		updateExpression();	
	}

	void ExpressionSolidCoordinate::fireZHeight(){
		editExpression(zHeightExpression, zHeight);
		updateExpression();	
	}	

	void ExpressionSolidCoordinate::cleanup(){
		NXOpen::PartCleanup *cleanup = NXOpen::Session::GetSession()->NewPartCleanup();
		cleanup->SetDeleteUnusedExpressions(true);
		cleanup->DoCleanup();
		delete cleanup;
	}

	void ExpressionSolidCoordinate::import(){
		NXOpen::PartImporter *importer = dynamic_cast<NXOpen::PartImporter *> (part->ImportManager()->CreatePartImporter());
		importer->SetFileName(filename.c_str());
		importer->SetScale(1.0);
		importer->SetCreateNamedGroup(true);
		importer->SetImportViews(false);
		importer->SetImportCamObjects(false);
		importer->SetLayerOption(NXOpen::PartImporter::LayerOptionTypeWork);
		importer->SetDestinationCoordinateSystemSpecification(NXOpen::PartImporter::DestinationCoordinateSystemSpecificationTypeWork);	
		NXOpen::Matrix3x3 element;
		element.Xx = 1.0;
		element.Xy = 0.0;
		element.Xz = 0.0;	  
		element.Yx = 0.0;
		element.Yy = 1.0;
		element.Yz = 0.0;	  
		element.Zx = 0.0;
		element.Zy = 0.0;
		element.Zz = 1.0;
		importer->SetDestinationCoordinateSystem(part->NXMatrices()->Create(element));		
		importer->SetDestinationPoint(NXOpen::Point3d(0.0, 0.0, 0.0));
		importer->Commit();
		importer->Destroy();
	}

	void ExpressionSolidCoordinate::bindExpression(){
		NXOpen::ExpressionCollection::iterator it = part->Expressions()->begin();
		for( ; it!=part->Expressions()->end(); ++it){			
			if(isDesiredExpression(*it, diameterComment)){
				diameterExpression = *it;
			}
			if(isDesiredExpression(*it, xHeightComment)){
				xHeightExpression = *it;
			}
			if(isDesiredExpression(*it, yHeightComment)){
				yHeightExpression = *it;
			}
			if(isDesiredExpression(*it, zHeightComment)){
				zHeightExpression = *it;
			}
		}
	}

	void ExpressionSolidCoordinate::initializeValue(){
		editExpression(diameterExpression, diameter);
		editExpression(xHeightExpression, xHeight);
		editExpression(yHeightExpression, yHeight);
		editExpression(zHeightExpression, zHeight);
		updateExpression();
	}

	bool ExpressionSolidCoordinate::isDesiredExpression(NXOpen::Expression *expression, const std::string comment){
		std::string equation(expression->Equation().GetText());
		if(equation.find(comment) != std::string::npos){			
			return true;
		}else{
			return false;
		}
	}

	void ExpressionSolidCoordinate::editExpression(NXOpen::Expression *expression, double value){
		char toValue[50] = "\0";
		toString(value, toValue);
		expression->SetRightHandSide(toValue);
	}	
}