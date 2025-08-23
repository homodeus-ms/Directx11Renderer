#pragma once

class Actor;

DECLARE_MULTICAST_DELEGATE(OnPopUpClosed);

class PopUpController : public enable_shared_from_this<PopUpController>
{
public:
	void BeginPlay();
	void CreatePopUpUI();
	void OnActorRegistered(weak_ptr<Actor> actor);

	OnPopUpClosed _onPopUpClosed;

private:
	string GetSliderName(const string& shown, const string& op1, int op2);
	void DrawActorControls();
	bool _popUpOpen = false;

	vector<weak_ptr<Actor>> _basicMaterialActors;
	vector<weak_ptr<Actor>> _pbrMaterialActors;
	vector<weak_ptr<Actor>> _reflectActors;

	vector<vector<shared_ptr<MaterialBase>>> _basicMaterials;
	vector<vector<shared_ptr<MaterialBase>>> _pbrMaterials;

	// Reflect Actor는 하나의 Material만 가진다고 가정
	vector<shared_ptr<MaterialBase>> _reflectMaterials;

};

