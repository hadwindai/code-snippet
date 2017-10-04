
#ifndef EXPRESSION_SOLID_COORDINATE_HXX_INCLUDED
#define EXPRESSION_SOLID_COORDINATE_HXX_INCLUDED

#include <uf.h>
#include <uf_modl.h>
#include "SolidCoordinate.hxx"

namespace HADWIN{

	class ExpressionSolidCoordinate: public SolidCoordinate{

	    private: static const std::string filename; 
		private: static const std::string diameterComment; 
		private: static const std::string xHeightComment; 
		private: static const std::string yHeightComment; 
		private: static const std::string zHeightComment; 

		private: NXOpen::Expression *diameterExpression;
		private: NXOpen::Expression *xHeightExpression;
		private: NXOpen::Expression *yHeightExpression;
		private: NXOpen::Expression *zHeightExpression;
					
		public:	ExpressionSolidCoordinate(NXOpen::Part *_part);

		public: ExpressionSolidCoordinate(NXOpen::Part *_part, double _diameter, double _height);

		public: ExpressionSolidCoordinate(NXOpen::Part *_part, double _diameter, double _xHeight, double _yHeight, double _zHeight);	

		public: virtual ~ExpressionSolidCoordinate();

		public: void create();		

		public:	void commit();

		public:	void remove();

		protected: void fireDiameter();

		protected: void fireHeight();

		protected: void fireXHeight();
			
		protected: void fireYHeight();

		protected: void fireZHeight();

		private: void cleanup();

		private: void import();

		private: void bindExpression();

		private: void initializeValue();

		private: bool isDesiredExpression(NXOpen::Expression *expression, const std::string comment);

		private: void editExpression(NXOpen::Expression *expression, double value);

		private: void updateExpression(){
			UF_MODL_update();
		}

	};
}
#endif