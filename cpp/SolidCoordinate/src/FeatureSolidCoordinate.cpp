
#include <uf_obj.h>
#include <uf_modl.h>
#include <NXOpen/Axis.hxx>
#include <NXOpen/Body.hxx>
#include <NXOpen/Face.hxx>
#include <NXOpen/Features_FeatureCollection.hxx>
#include <NXOpen/Features_SphereBuilder.hxx>
#include <NXOpen/Features_CylinderBuilder.hxx>
#include <NXOpen/Features_ConeBuilder.hxx>
#include <NXOpen/PointCollection.hxx>
#include <NXOpen/DirectionCollection.hxx>
#include <NXOpen/Facet_FacetedBodyCollection.hxx>
#include <NXOpen/DisplayManager.hxx>
#include <NXOpen/DisplayModification.hxx>
#include <NXOpen/DisplayableObject.hxx>
#include <NXOpen/Preferences_SessionModeling.hxx>
#include <NXOpen/Preferences_SessionPreferences.hxx>
#include <NXOpen/Update.hxx>

#include "FeatureSolidCoordinate.hxx"

namespace HADWIN{

		FeatureSolidCoordinate::FeatureSolidCoordinate(NXOpen::Part *_part): 			
			SolidCoordinate(_part)			
		{			
		}

		FeatureSolidCoordinate::FeatureSolidCoordinate(NXOpen::Part *_part, double _diameter, double _height):
			SolidCoordinate(_part, _diameter, _height)
		{			
		}

		FeatureSolidCoordinate::FeatureSolidCoordinate(NXOpen::Part *_part, double _diameter, double _xHeight, double _yHeight, double _zHeight):
			SolidCoordinate(_part, _diameter, _xHeight, _yHeight, _zHeight)
		{		
		}	

		FeatureSolidCoordinate::~FeatureSolidCoordinate(){
			delete origin;
			delete xCylinder;
			delete yCylinder;
			delete zCylinder;
			delete xCone;
			delete yCone;
			delete zCone;
		}
		
		void FeatureSolidCoordinate::create(){
			origin = createSphere();
			paintColor(origin, 87);
			xCylinder = createCylinder(xHeight, NXOpen::Vector3d(1.0, 0.0, 0.0));
			paintColor(xCylinder, 186);
			xCone = createCone(NXOpen::Point3d(xHeight, 0.0, 0.0), NXOpen::Vector3d(1.0, 0.0, 0.0));
			paintColor(xCone, 186);
			yCylinder = createCylinder(yHeight, NXOpen::Vector3d(0.0, 1.0, 0.0));
			paintColor(yCylinder, 108);
			yCone = createCone(NXOpen::Point3d(0.0, yHeight, 0.0), NXOpen::Vector3d(0.0, 1.0, 0.0));
			paintColor(yCone, 108);
			zCylinder = createCylinder(zHeight, NXOpen::Vector3d(0.0, 0.0, 1.0));
			paintColor(zCylinder, 211);
			zCone = createCone(NXOpen::Point3d(0.0, 0.0, zHeight), NXOpen::Vector3d(0.0, 0.0, 1.0));
			paintColor(zCone, 211);
		}
		
		void FeatureSolidCoordinate::commit(){
			// add action to unit 
			//do some actions that user can't edit the soild later.

		}

		void FeatureSolidCoordinate::remove(){
			uf_list_p_t list;
			UF_MODL_create_list(&list);
			UF_MODL_put_list_item(list, origin->Tag());
			UF_MODL_put_list_item(list, xCylinder->Tag());
			UF_MODL_put_list_item(list, yCylinder->Tag());
			UF_MODL_put_list_item(list, zCylinder->Tag());
			UF_MODL_put_list_item(list, xCone->Tag());
			UF_MODL_put_list_item(list, yCone->Tag());
			UF_MODL_put_list_item(list, zCone->Tag());
			UF_MODL_delete_feature(list);
			UF_MODL_delete_list(&list);
		}

		void FeatureSolidCoordinate::fireDiameter(){
			fireSphereDiameter();
			fireCylinderDiameter(xCylinder);
			fireCylinderDiameter(yCylinder);
			fireCylinderDiameter(zCylinder);
			fireConeDiameter(xCone);
			fireConeDiameter(yCone);
			fireConeDiameter(zCone);
			updateFeatures();
		}

		void FeatureSolidCoordinate::fireHeight(){
			fireCylinderHeight(xCylinder, xHeight);	
			fireConePoint(xCone, NXOpen::Point3d(xHeight, 0.0, 0.0));
			fireCylinderHeight(yCylinder, yHeight);
			fireConePoint(yCone, NXOpen::Point3d(0.0, yHeight, 0.0));
			fireCylinderHeight(zCylinder, zHeight);
			fireConePoint(zCone, NXOpen::Point3d(0.0, 0.0, zHeight));
			updateFeatures();
	    }

		void FeatureSolidCoordinate::fireXHeight(){			
			fireCylinderHeight(xCylinder, xHeight);	
			fireConePoint(xCone, NXOpen::Point3d(xHeight, 0.0, 0.0));
			updateFeatures();
	    }
		
		void FeatureSolidCoordinate::fireYHeight(){
			fireCylinderHeight(yCylinder, yHeight);
			fireConePoint(yCone, NXOpen::Point3d(0.0, yHeight, 0.0));
			updateFeatures();
	    }

		void FeatureSolidCoordinate::fireZHeight(){
			fireCylinderHeight(zCylinder, zHeight);
			fireConePoint(zCone, NXOpen::Point3d(0.0, 0.0, zHeight));
			updateFeatures();
	    }

		NXOpen::Features::Sphere *FeatureSolidCoordinate::createSphere(){			
			 char toDiameter[50] = "\0";			
			 toString(diameter, toDiameter);
			 NXOpen::Features::SphereBuilder *builder = part->Features()->CreateSphereBuilder(NULL);	
			 builder->SetCenterPoint(part->Points()->CreatePoint(NXOpen::Point3d(0.0, 0.0, 0.0)));
			 builder->Diameter()->SetRightHandSide(toDiameter);			 
			 NXOpen::Features::Sphere *sphere = dynamic_cast<NXOpen::Features::Sphere *>(builder->CommitFeature());
			 builder->Destroy();
			 part->FacetedBodies()->DeleteTemporaryFacesAndEdges();
			 return sphere;
		}

		NXOpen::Features::Cylinder *FeatureSolidCoordinate::createCylinder(const double &_height, const NXOpen::Vector3d &vector){
			 char toHeight[50] = "\0";
			 char toDiameter[50] = "\0";
			 toString(_height, toHeight);
			 toString(diameter, toDiameter);
			 NXOpen::Features::CylinderBuilder *builder = part->Features()->CreateCylinderBuilder(NULL);	
			 builder->Height()->SetRightHandSide(toHeight);
			 builder->Diameter()->SetRightHandSide(toDiameter);
			 NXOpen::Axis *axis = builder->Axis();	
			 axis->SetPoint(part->Points()->CreatePoint(NXOpen::Point3d(0.0, 0.0, 0.0)));
			 axis->SetDirection(part->Directions()->CreateDirection(NXOpen::Point3d(0.0, 0.0, 0.0), vector, NXOpen::SmartObject::UpdateOptionWithinModeling));	
			 NXOpen::Features::Cylinder *cylinder = dynamic_cast<NXOpen::Features::Cylinder *>(builder->CommitFeature());
			 builder->Destroy();
			 part->FacetedBodies()->DeleteTemporaryFacesAndEdges();
			 return cylinder;
		 }

		NXOpen::Features::Cone *FeatureSolidCoordinate::createCone(const NXOpen::Point3d &point, const NXOpen::Vector3d &vector){
			 char toHeight[50] = "\0";
			 char toDiameter[50] = "\0";	
			 toString(diameter*1.5, toHeight);
			 toString(diameter*2.0, toDiameter);
			 NXOpen::Features:: ConeBuilder *builder = part->Features()->CreateConeBuilder(NULL);
			 builder->Height()->SetRightHandSide(toHeight);
			 builder->BaseDiameter()->SetRightHandSide(toDiameter);
			 builder->TopDiameter()->SetRightHandSide("0.0");
			 NXOpen::Axis *axis = builder->Axis();	
			 axis->SetPoint(part->Points()->CreatePoint(point));
			 axis->SetDirection(part->Directions()->CreateDirection(point, vector, NXOpen::SmartObject::UpdateOptionWithinModeling));	
			 NXOpen::Features::Cone *cone = dynamic_cast<NXOpen::Features::Cone *>(builder->CommitFeature());
			 builder->Destroy();
			 part->FacetedBodies()->DeleteTemporaryFacesAndEdges();
			 return cone;
		 }

		void FeatureSolidCoordinate::fireSphereDiameter(){
			  char toDiameter[50] = "\0";			
			  toString(diameter, toDiameter);
			  NXOpen::Features::SphereBuilder *builder = part->Features()->CreateSphereBuilder(origin);
			  builder->Diameter()->SetRightHandSide(toDiameter);			
			  builder->Commit();	
			  builder->Destroy();
		}

		void FeatureSolidCoordinate::fireCylinderDiameter(NXOpen::Features::Cylinder *cylinder){
			  char toDiameter[50] = "\0";			
			  toString(diameter, toDiameter);			 
			  NXOpen::Features::CylinderBuilder *builder = part->Features()->CreateCylinderBuilder(cylinder);
			  builder->Diameter()->SetRightHandSide(toDiameter);			
			  builder->Commit();	
			  builder->Destroy();
		}

		void FeatureSolidCoordinate::fireConeDiameter(NXOpen::Features::Cone *cone){
			 char toHeight[50] = "\0";
			 char toDiameter[50] = "\0";	
			 toString(diameter*1.5, toHeight);
			 toString(diameter*2.0, toDiameter);
			 NXOpen::Features:: ConeBuilder *builder = part->Features()->CreateConeBuilder(cone);
			 builder->Height()->SetRightHandSide(toHeight);
			 builder->BaseDiameter()->SetRightHandSide(toDiameter);			
			 builder->CommitFeature();
			 builder->Destroy();
		}

		 void FeatureSolidCoordinate::fireCylinderHeight(NXOpen::Features::Cylinder *cylinder, double _height){		
			  char toHeight[50] = "\0";			
			  toString(_height, toHeight);			 
			  NXOpen::Features::CylinderBuilder *builder = part->Features()->CreateCylinderBuilder(cylinder);
			  builder->Height()->SetRightHandSide(toHeight);			
			  builder->Commit();	
			  builder->Destroy();		  
		 }

		 void FeatureSolidCoordinate::fireConePoint(NXOpen::Features::Cone *cone, NXOpen::Point3d &point){
			 NXOpen::Features:: ConeBuilder *builder = part->Features()->CreateConeBuilder(cone);			 
			 builder->Axis()->SetPoint(part->Points()->CreatePoint(point));			 
			 builder->CommitFeature();
			 builder->Destroy();
		 }

		 void FeatureSolidCoordinate::paintColor(NXOpen::Features::BodyFeature *bodyFeature, int color){		
			 tag_t tag = NULL_TAG;
			 UF_MODL_ask_feat_body(bodyFeature->GetTag(), &tag);
			 UF_OBJ_set_color(tag, color);			
		 }

}