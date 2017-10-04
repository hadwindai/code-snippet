
#ifndef DRAWING_SOLID_COORDINATE_HXX_INCLUDED
#define DRAWING_SOLID_COORDINATE_HXX_INCLUDED

#include "SolidCoordinate.hxx"

namespace HADWIN{

	class DrawingSolidCoordinate: public SolidCoordinate{

		public:	DrawingSolidCoordinate(NXOpen::Part *_part);

		public: DrawingSolidCoordinate(NXOpen::Part *_part, double _diameter, double _height);

		public: DrawingSolidCoordinate(NXOpen::Part *_part, double _diameter, double _xHeight, double _yHeight, double _zHeight);	

		public:	virtual ~DrawingSolidCoordinate();

		public: void create();		

		public:	void commit();

		public:	void remove();

		protected: void fireDiameter(){}

		protected: void fireHeight(){}

		protected: void fireXHeight(){}

		protected: void fireYHeight(){}

		protected: void fireZHeight(){}	

	};
}
#endif