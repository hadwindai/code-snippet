
#include "DrawingSolidCoordinate.hxx"

namespace HADWIN{

	DrawingSolidCoordinate::DrawingSolidCoordinate(NXOpen::Part *_part):
		SolidCoordinate(_part)
	{
	}

	DrawingSolidCoordinate::DrawingSolidCoordinate(NXOpen::Part *_part, double _diameter, double _height):
		SolidCoordinate(_part, _diameter, _height)
	{			
	}

	DrawingSolidCoordinate::DrawingSolidCoordinate(NXOpen::Part *_part, double _diameter, double _xHeight, double _yHeight, double _zHeight):
		SolidCoordinate(_part, _diameter, _xHeight, _yHeight, _zHeight)
	{		
	}	

	DrawingSolidCoordinate::~DrawingSolidCoordinate(){

	}

	void DrawingSolidCoordinate::create(){
	
	}	

	void DrawingSolidCoordinate::commit(){
	
	}

	void DrawingSolidCoordinate::remove(){
	
	
	}
}