#pragma once

class Actor;
class Widget_Slider4;

class SubWindowController : public enable_shared_from_this<SubWindowController>
{
public:
	void BeginPlay();
	void CreateSubWindow();

	void OnActorRegistered(weak_ptr<Actor> actor);

private:
	void DrawActorControls();

	vector<weak_ptr<Actor>> _basicMaterialActors;
	vector<weak_ptr<Actor>> _pbrMaterialActors;
	vector<vector<shared_ptr<MaterialBase>>> _basicMaterials;
	vector<vector<shared_ptr<MaterialBase>>> _pbrMaterials;

	vector<vector<Widget_Slider4*>> _ADSEForBasicMaterials;
	

};

