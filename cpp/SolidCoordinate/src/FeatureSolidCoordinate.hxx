
#ifndef FEATURE_SOLID_COORDINATE_HXX_INCLUDED
#define FEATURE_SOLID_COORDINATE_HXX_INCLUDED

#include <NXOpen/Features_Cylinder.hxx>
#include <NXOpen/Features_Cone.hxx>
#include <NXOpen/Features_Sphere.hxx>
#include <NXOpen/Facet_FacetedBodyCollection.hxx>
#include "SolidCoordinate.hxx"

namespace HADWIN{

	class FeatureSolidCoordinate: public SolidCoordinate{

		private: NXOpen::Features::Sphere *origin;
		private: NXOpen::Features::Cylinder *xCylinder;
		private: NXOpen::Features::Cylinder *yCylinder;
		private: NXOpen::Features::Cylinder *zCylinder;
		private: NXOpen::Features::Cone *xCone;
		private: NXOpen::Features::Cone *yCone;
		private: NXOpen::Features::Cone *zCone;

		public:	FeatureSolidCoordinate(NXOpen::Part *_part);

		public: FeatureSolidCoordinate(NXOpen::Part *_part, double _diameter, double _height);

		public: FeatureSolidCoordinate(NXOpen::Part *_part, double _diameter, double _xHeight, double _yHeight, double _zHeight);	

		public:	virtual ~FeatureSolidCoordinate();

		public: void create();		

		public:	void commit();

		public:	void remove();

		protected: void fireDiameter();

	    protected: void fireHeight();

		protected: void fireXHeight();
		
		protected: void fireYHeight();

		protected: void fireZHeight();	

	   // Create
	    private: NXOpen::Features::Sphere *createSphere();

		private: NXOpen::Features::Cylinder *createCylinder(const double &_height, const NXOpen::Vector3d &vector);

		private: NXOpen::Features::Cone *createCone(const NXOpen::Point3d &point, const NXOpen::Vector3d &vector);		

		// Fire Diameter
		private: void fireSphereDiameter();

	    private: void fireCylinderDiameter(NXOpen::Features::Cylinder *cylinder);

		private: void fireConeDiameter(NXOpen::Features::Cone *cone);

		// Fire Height
	    private: void fireCylinderHeight(NXOpen::Features::Cylinder *cylinder, double _height);

		private: void fireConePoint(NXOpen::Features::Cone *cone, NXOpen::Point3d &point);

		// Color
		private: void paintColor(NXOpen::Features::BodyFeature *bodyFeature, int color);		

		private: void updateFeatures(){
			zCone->MakeCurrentFeature();
			part->FacetedBodies()->DeleteTemporaryFacesAndEdges();
		}


	};
}
#endif
