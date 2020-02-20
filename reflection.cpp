#include"RigidBody.h"

void RangeRectInit(vector<CollisionRect>& RangeRect, float const& AppW, float const& AppH);

void RangeCircleInit(vector<RigidBody>& RangeCircle, float const& AppW, float const& AppH);

void BallsInit(vector<RigidBody>& BallsArray, RigidBody const& Ball_tmp, float const& AppW, float const& AppH);

void HitWhiteBall(sf::RenderWindow& App, RigidBody& WhiteBall, bool& isStatic, Cue& cue);

int reflection_main()
{
	const float AppW = 2200, AppH = 1300;
	sf::RenderWindow App(sf::VideoMode((unsigned int)AppW, (unsigned int)AppH), "Reflection", CannotResize);
	sf::Vector2f ViewScale{ 1.1f,1.1f };
	sf::View MainView(sf::Vector2f{ AppW / 2,AppH / 2 },
		sf::Vector2f{ AppW * ViewScale.x,AppH * ViewScale.y });
	AssetManager AM;
	//////////////////////////////////////////////////////////
	////  Balls:
	vector<RigidBody>BallsArray;
	//WhiteBall:
	RigidBody Ball_tmp("WhiteBall");
	Ball_tmp.SetdX({ 0,0 });
	Ball_tmp.SetFillColor(sf::Color::White);
	Ball_tmp.SetPos(sf::Vector2f{ AppW / 2,AppH / 2 });
	BallsArray.push_back(Ball_tmp);
	// Balls Object:
	BallsInit(BallsArray, Ball_tmp, AppW, AppH);
	RigidBodys Balls(BallsArray);
	//////////////////////////////////////////////////////////
	////  RangeCiecle
	vector<RigidBody>RangeCircle;
	//RangeCircleInit(RangeCircle, AppW, AppH);
	//////////////////////////////////////////////////////////
	////  RangeRect
	vector<CollisionRect>RangeRects;
	RangeRectInit(RangeRects, AppW, AppH);
	//////////////////////////////////////////////////////////
	////  BackGround
	sf::RectangleShape BGRect({ AppW,AppH });
	BGRect.setFillColor(sf::Color{ 90,140,90,255 });
	BGRect.setOutlineColor(sf::Color{ 10,10,10, 255 });
	BGRect.setOutlineThickness(20);
	BGRect.setOrigin(BGRect.getSize().x / 2, BGRect.getSize().y / 2);
	BGRect.setPosition(AppW / 2, AppH / 2);
	//////////////////////////////////////////////////////////
	//  Set Frames
	float Frames = 140.0f;
	long TimeDelay = (long)(1000.0f / Frames);
	time_t StartTime = 0;
	time_t EndTime = 0;
	//////////////////////////////////////////////////////////
	//  Set Font
	sf::Font TDFont = AssetManager::GetFont(font_3D);
	sf::Text text;
	text.setFont(TDFont);
	text.setCharacterSize(18);
	text.setFillColor(sf::Color::Red);
	text.setPosition(50, 50);
	string textin;
	//////////////////////////////////////////////////////////
	//  Set Animate Delta Time
	sf::Time dt;
	sf::Clock CL;
	//////////////////////////////////////////////////////////
	////  Hit White Ball Thread
	bool isStatic = false;
	isStatic = true;
	Cue cue;
	thread HitWhiteBall_Thread(HitWhiteBall, ref(App), ref(Ball_tmp), ref(isStatic), ref(cue));

	while (App.isOpen())
	{
		StartTime = clock();
		sf::Event ev;
		while (App.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed) {
				App.close();
			}
			if (KeyPressing and KeyEvent(Escape)) {
				App.close();
			}
		}
		dt = CL.restart();

		Balls.SetWhiteBall(Ball_tmp);

		Balls.UpdateAxis(RangeCircle, RangeRects);

		App.clear(sf::Color{ 90,90,90,255 });

		App.setView(MainView);

		App.draw(BGRect);

		for (auto it = RangeCircle.begin(); it != RangeCircle.end(); it++)
		{
			App.draw(*it);
		}

		App.draw(Balls);

		App.draw(text);

		App.display();

		Ball_tmp = Balls.GetWhiteBall();

		isStatic = (Balls.GetStatic() and Ball_tmp.GetdX() == Vector{ 0,0 });

		//////////////////////////////////////////////////////////
		//  Set FPS
		EndTime = clock();
		while (EndTime - StartTime <= TimeDelay) {
			EndTime = clock();
		}
		//////////////////////////////////////////////////////////
		//  Get FPS
		int frametmp = (int)(1000.0f / (EndTime - StartTime));
		textin = ToString(frametmp);
		textin += "fps\n";
		text.setPosition(App.mapPixelToCoords(sf::Vector2i{ 50,50 }));
		text.setScale(ViewScale);
		text.setString(textin);
	}

	HitWhiteBall_Thread.join();

	return 0;
}

void RangeRectInit(vector<CollisionRect>& RangeRect, float const& AppW, float const& AppH)
{
	string RectName = "";
	RectName = "Rect." + ToString(1);
	CollisionRect rangerect_tmp{ RectName,sf::FloatRect{0,0,AppW,5} }; //  Up
	RangeRect.push_back(rangerect_tmp);

	RectName = "Rect." + ToString(2);
	rangerect_tmp = { RectName, sf::FloatRect{ 0,0,5,AppH } };            //  Left
	RangeRect.push_back(rangerect_tmp);

	RectName = "Rect." + ToString(3);
	rangerect_tmp = { RectName, sf::FloatRect{ 0,AppH - 5,AppW,5 } };	   //  Down
	RangeRect.push_back(rangerect_tmp);

	RectName = "Rect." + ToString(4);
	rangerect_tmp = { RectName, sf::FloatRect{ AppW - 5,0,5,AppH } };     //  Right
	RangeRect.push_back(rangerect_tmp);
}

void RangeCircleInit(vector<RigidBody>& RangeCircle, float const& AppW, float const& AppH)
{
	string RangeName = "";
	RigidBody rangecircle_tmp("");
	RangeName = "Range." + ToString(1); rangecircle_tmp.SetName(RangeName);
	rangecircle_tmp.SetPos(sf::Vector2f{ 0,0 });
	RangeCircle.push_back(rangecircle_tmp);

	RangeName = "Range." + ToString(2); rangecircle_tmp.SetName(RangeName);
	rangecircle_tmp.SetPos(sf::Vector2f{ AppW,0 });
	RangeCircle.push_back(rangecircle_tmp);

	RangeName = "Range." + ToString(3); rangecircle_tmp.SetName(RangeName);
	rangecircle_tmp.SetPos(sf::Vector2f{ AppW,AppH });
	RangeCircle.push_back(rangecircle_tmp);

	RangeName = "Range." + ToString(4); rangecircle_tmp.SetName(RangeName);
	rangecircle_tmp.SetPos(sf::Vector2f{ 0,AppH });
	RangeCircle.push_back(rangecircle_tmp);
}

void BallsInit(vector<RigidBody>& BallsArray, RigidBody const& Ball_tmp,float const& AppW, float const& AppH)
{
	RigidBody WhiteBall = Ball_tmp;
	srand((unsigned int)time(NULL));

	string newName = "";
	for (int i = 0; i < 15;)
	{
		WhiteBall.SetdX({ (float)(rand() % 16 - 8),(float)(rand() % 16 - 8) });
		WhiteBall.SetPos(sf::Vector2f{ rand() % (int)(AppW / 2) + AppW / 10,
			rand() % (int)(AppH / 2) + AppH / 10 });
		WhiteBall.SetFillColor(sf::Color{ (sf::Uint8)(rand() % 200),
			(sf::Uint8)(rand() % 200),(sf::Uint8)(rand() % 200),255 });
		auto it = BallsArray.begin();
		for (; it != BallsArray.end(); it++)
		{
			if (WhiteBall.InitisLegal(*it) == false)
			{
				break;
			}
		}
		if (it == BallsArray.end() and WhiteBall.GetFiilColor() != sf::Color{ 90,140,90,255 })
		{
			newName = "ColorBalls_No." + ToString(i + 1);
			WhiteBall.SetName(newName);
			cout << newName << endl;
			BallsArray.push_back(WhiteBall);
			i++;
		}
	}
}

bool isInWindowRange(Vector target, Vector windowrange)
{
	if (target.x >= 0 and target.x <= windowrange.x and target.y >= 0 and target.y <= windowrange.y)
	{
		return true;
	}
	return false;
}

void HitWhiteBall(sf::RenderWindow& App, RigidBody& WhiteBall, bool& isStatic, Cue& cue)
{
	bool isClicked = false;
	bool isCanceled = false;
	auto MousePos = sf::Mouse::getPosition(App);
	auto AppRange = App.getSize();
	while (App.isOpen())
	{
		if (KeyEvent(Space))cout << "Pause" << endl;
		MousePos = sf::Mouse::getPosition(App);
		if (isStatic == true) //  if all balls static
		{
			Vector DistanceAxis;
			Vector NewVelocity;
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) == true 
				and isInWindowRange(MousePos, AppRange) == true)
			{
				while ((sf::Mouse::isButtonPressed(sf::Mouse::Left) == true))
				{
					//  Do Change
					if (isCanceled == false)
					{
						//  Do some to change
						//cout << "Doing some thing..." << endl;
						MousePos = sf::Mouse::getPosition(App);
						MousePos = (sf::Vector2i)App.mapPixelToCoords(sf::Vector2i{ MousePos.x, MousePos.y });
						DistanceAxis = static_cast<Vector>(WhiteBall.GetPos()) - MousePos;
						cue.m_dir = DistanceAxis;
						cue.isViewed = true;
						NewVelocity = DistanceAxis / 40;
						isClicked = true;
					}
					if (sf::Mouse::isButtonPressed(sf::Mouse::Right) == true)
					{
						//Cancel Change
						if (isCanceled == false)
						{
							//  Do some to cancel
							//cout << "Cancel doing..." << endl;
							cue.isViewed = false;
							isClicked = false;
							isCanceled = true;
						}
					}
				}
			}
			
			//  Commit Changes
			if (isClicked == true and isCanceled == false)
			{
				//  Do some to Commit
				//cout << "Change Has been Commited." << endl;
				WhiteBall.SetdX(NewVelocity);
				cue.isViewed = false;
			}
			isClicked = false;
			isCanceled = false;
		}
	}
}