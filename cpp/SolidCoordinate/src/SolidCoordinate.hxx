
#ifndef SOLID_COORDINATE_HXX_INCLUDED
#define SOLID_COORDINATE_HXX_INCLUDED

#include <NXOpen/Part.hxx>

namespace HADWIN{

	class SolidCoordinate{

		protected: NXOpen::Part *part;
		protected: double diameter;
		protected: double xHeight;
		protected: double yHeight;
		protected: double zHeight;

		protected: SolidCoordinate(NXOpen::Part *_part): 			
			part(_part),
			diameter(10.0),
			xHeight(50.0),
			yHeight(50.0),
			zHeight(50.0)
		{			
		}
		protected: SolidCoordinate(NXOpen::Part *_part, double _diameter, double _height):
			part(_part),
			diameter(_diameter),
			xHeight(_height),
			yHeight(_height),
			zHeight(_height)
		{			
		}
		protected: SolidCoordinate(NXOpen::Part *_part, double _diameter, double _xHeight, double _yHeight, double _zHeight):
			part(_part),
			diameter(_diameter),
			xHeight(_xHeight),
			yHeight(_yHeight),
			zHeight(_zHeight)
		{		
		}	

		public:	virtual ~SolidCoordinate(){};
		

		public:	NXOpen::Part *getPart(){
			return part;
		}

		public:	double getDiameter(){
			return diameter;
		}
		public:	void setDiameter(double _diameter){
			diameter = _diameter;					
		}
		public:	void updateDiameter(double _diameter){
			diameter = _diameter;
			fireDiameter();					
		}

		public:	double getXHeight(){
			return xHeight;
		}
		public:	void setXHeight(double _xHeight){
			xHeight = _xHeight;			
		}
		public:	void updateXHeight(double _xHeight){
			xHeight = _xHeight;
			fireXHeight();
		}

		public:	double getYHeight(){
			return yHeight;
		}
		public:	void setYHeight(double _yHeight){
			yHeight = _yHeight;			
		}
		public:	void updateYHeight(double _yHeight){
			yHeight = _yHeight;
			fireYHeight();
		}

		public:	double getZHeight(){
			return zHeight;
		}
		public:	void setZHeight(double _zHeight){
			zHeight = _zHeight;			
		}
		public:	void updateZHeight(double _zHeight){
			zHeight = _zHeight;
			fireZHeight();
		}

		public:	void setHeight(double _height){
			xHeight = _height;	
			yHeight = _height;	
			zHeight = _height;	
		}
		public:	void updateHeight(double _height){
			xHeight = _height;	
			yHeight = _height;	
			zHeight = _height;	
			fireHeight();
		}

		public:	virtual void create() = 0;		

		public:	virtual void commit() = 0;

		public:	virtual void remove() = 0;

		protected: virtual void fireDiameter() = 0;		

		protected: virtual void fireXHeight() = 0;
		
		protected: virtual void fireYHeight() = 0;

		protected: virtual void fireZHeight() = 0;	

	    protected: virtual void fireHeight() = 0;		

		protected: void toString(const double &value, char *toValue){
			sprintf(toValue, "%.2f", value);
		}

	};
}

#endif
