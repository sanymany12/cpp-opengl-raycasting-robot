#include "MyApp.h"
#include "ObjParser.h"
#include "SDL_GLDebugMessageCallback.h"

#include <imgui.h>

#include "ZHUtils.h"

CMyApp::CMyApp() : m_cameraManipulator(m_camera)
{
}

CMyApp::~CMyApp()
{
}

void CMyApp::SetupDebugCallback()
{
	// engedélyezzük és állítsuk be a debug callback függvényt ha debug context-ben vagyunk
	GLint context_flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
	if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
		glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, GL_DONT_CARE, 0, nullptr, GL_FALSE);
		glDebugMessageCallback(SDL_GLDebugMessageCallback, nullptr);
	}
}

void CMyApp::InitShaders()
{
	m_programID = glCreateProgram();
	AttachShader(m_programID, GL_VERTEX_SHADER, "Shaders/Vert_PosNormTex.vert");
	AttachShader(m_programID, GL_FRAGMENT_SHADER, "Shaders/Frag_LightingRobot.frag");
	LinkProgram(m_programID);
}

void CMyApp::CleanShaders()
{
	glDeleteProgram(m_programID);
}

void CMyApp::InitGeometry()
{
	const std::initializer_list<VertexAttributeDescriptor> vertexAttribList =
	{
		{0, offsetof(Vertex, position), 3, GL_FLOAT},
		{1, offsetof(Vertex, normal), 3, GL_FLOAT},
		{2, offsetof(Vertex, texcoord), 2, GL_FLOAT},
	};

	m_cubeGPU = CreateGLObjectFromMesh(createCube(), vertexAttribList);

	// újrahasznált mesh objektum 
	MeshObject<Vertex> mesh;

	mesh = ObjParser::parse("Assets/head.obj");
	m_robotHeadGPU = CreateGLObjectFromMesh(mesh, vertexAttribList);

	mesh = ObjParser::parse("Assets/torso.obj");
	m_robotTorsoGPU = CreateGLObjectFromMesh(mesh, vertexAttribList);

	mesh = ObjParser::parse("Assets/right_upper_arm.obj");
	m_robotRightArmUpperGPU = CreateGLObjectFromMesh(mesh, vertexAttribList);

	mesh = ObjParser::parse("Assets/right_lower_arm.obj");
	m_robotRightArmLowerGPU = CreateGLObjectFromMesh(mesh, vertexAttribList);

	mesh = ObjParser::parse("Assets/left_upper_arm.obj");
	m_robotLeftArmUpperGPU = CreateGLObjectFromMesh(mesh, vertexAttribList);

	mesh = ObjParser::parse("Assets/left_lower_arm.obj");
	m_robotLeftArmLowerGPU = CreateGLObjectFromMesh(mesh, vertexAttribList);

	mesh = ObjParser::parse("Assets/leg.obj");
	m_robotLegGPU = CreateGLObjectFromMesh(mesh, vertexAttribList);
}

void CMyApp::CleanGeometry()
{
	CleanOGLObject(m_cubeGPU);
	CleanOGLObject(m_robotHeadGPU);
	CleanOGLObject(m_robotTorsoGPU);
	CleanOGLObject(m_robotRightArmUpperGPU);
	CleanOGLObject(m_robotRightArmLowerGPU);
	CleanOGLObject(m_robotLeftArmUpperGPU);
	CleanOGLObject(m_robotLeftArmLowerGPU);
	CleanOGLObject(m_robotLegGPU);
}

void CMyApp::InitTextures()
{
	glCreateSamplers(1, &m_SamplerID);
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	ImageRGBA woodImage = ImageFromFile("Assets/Wood_Table_Texture.png");

	glCreateTextures(GL_TEXTURE_2D, 1, &m_woodTextureID);
	glTextureStorage2D(m_woodTextureID, NumberOfMIPLevels(woodImage), GL_RGBA8, woodImage.width, woodImage.height);
	glTextureSubImage2D(m_woodTextureID, 0, 0, 0, woodImage.width, woodImage.height, GL_RGBA, GL_UNSIGNED_BYTE, woodImage.data());

	glGenerateTextureMipmap(m_woodTextureID);

	ImageRGBA robotImage = ImageFromFile("Assets/Robot_Texture.png");

	glCreateTextures(GL_TEXTURE_2D, 1, &m_robotTextureID);
	glTextureStorage2D(m_robotTextureID, NumberOfMIPLevels(robotImage), GL_RGBA8, robotImage.width, robotImage.height);
	glTextureSubImage2D(m_robotTextureID, 0, 0, 0, robotImage.width, robotImage.height, GL_RGBA, GL_UNSIGNED_BYTE, robotImage.data());

	glGenerateTextureMipmap(m_robotTextureID);

	ImageRGBA shineImage = ImageFromFile("Assets/shine.png");

	glCreateTextures(GL_TEXTURE_2D, 1, &m_shineTextureID);
	glTextureStorage2D(m_shineTextureID, NumberOfMIPLevels(shineImage), GL_RGBA8, shineImage.width, shineImage.height);
	glTextureSubImage2D(m_shineTextureID, 0, 0, 0, shineImage.width, shineImage.height, GL_RGBA, GL_UNSIGNED_BYTE, shineImage.data());

	glGenerateTextureMipmap(m_shineTextureID);
}

void CMyApp::CleanTextures()
{
	glDeleteTextures(1, &m_woodTextureID);
	glDeleteTextures(1, &m_robotTextureID);
	glDeleteTextures(1, &m_shineTextureID);

	glDeleteSamplers( 1, &m_SamplerID );
}

bool CMyApp::Init()
{
	SetupDebugCallback();

	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	InitShaders();
	InitGeometry();
	InitTextures();

	//
	// egyéb inicializálás
	//

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hátrafelé néző lapok eldobását 
	glCullFace(GL_BACK);    // GL_BACK: a kamerától "elfelé" néző lapok, GL_FRONT: a kamera felé néző lapok 

	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás) 

	// kamera 
	m_camera.SetView(
		glm::vec3(0.0, 60.0, 150.0), // honnan nézzük a színteret  - eye
		glm::vec3(0.0, 0.0, 0.0),    // a színtér melyik pontját nézzük  - at
		glm::vec3(0.0, 1.0, 0.0));   // felfelé mutató irány a világban  - up

	m_cameraManipulator.SetStateFromCamera();

	return true;
}

void CMyApp::Clean()
{
	CleanShaders();
	CleanGeometry();
	CleanTextures();
}

Ray CMyApp::CalculatePixelRay(glm::vec2 pixel) const
{
	// NDC koordináták kiszámítása 
	glm::vec3 pickedNDC = glm::vec3(
		2.0f * (pixel.x + 0.5f) / m_windowSize.x - 1.0f,
		1.0f - 2.0f * (pixel.y + 0.5f) / m_windowSize.y, 0.0f);

	// A világ koordináták kiszámítása az inverz ViewProj mátrix segítségével
	glm::vec4 pickedWorld = glm::inverse(m_camera.GetViewProj()) * glm::vec4(pickedNDC, 1.0f);
	pickedWorld /= pickedWorld.w; // homogén osztás 
	Ray ray;

	// Raycasting kezdőpontja a kamera pozíciója 
	ray.origin = m_camera.GetEye();
	// Iránya a kamera pozíciójából a kattintott pont világ koordinátái felé 
	// FIGYELEM: NEM egység hosszúságú vektor! 
	ray.direction = glm::vec3(pickedWorld) - ray.origin;
	return ray;
}

bool CMyApp::intersectTableTop(const Ray& ray, glm::vec3& result)
{
	// sík 
	glm::vec3 planeQ = TABLE_POS + 0.5f * glm::vec3(-TABLE_SIZE.x, TABLE_SIZE.y, -TABLE_SIZE.z);
	glm::vec3 planeI = glm::vec3(TABLE_SIZE.x, 0.0f, 0.0f);
	glm::vec3 planeJ = glm::vec3(0.0f, 0.0f, TABLE_SIZE.z);
	Intersection inter;
	// ha van találat és az a négyzeten belül van 
	if (HitPlane(ray, planeQ, planeI, planeJ, inter)
		&& (inter.uv.x >= 0 && inter.uv.x <= 1)
		&& (inter.uv.y >= 0 && inter.uv.y <= 1)
		) {
		
		result = inter.position;
		return true;
	}
	else {
		return false;
	}
}

void CMyApp::Update(const SUpdateInfo& updateInfo)
{
	m_ElapsedTimeInSec = updateInfo.ElapsedTimeInSec;

	if (m_IsPicking) {
		// sugár indítása a kattintott pixelen át 
		Ray ray = CalculatePixelRay(glm::vec2(m_PickedPixel.x, m_PickedPixel.y));
		// metszés az asztallappal 
		glm::vec3 desiredPos;
		if (intersectTableTop(ray, desiredPos)) {
			CalculateRobotMovementData(desiredPos);
		}

        m_IsPicking = false;
	}

	CalculateRobotPosition(updateInfo.DeltaTimeInSec);

	if ( m_isTopCamera )
	{
        glm::vec3 camPosInObjSpace = glm::vec3( 0.0f, 60.0f, -150.0f );
        glm::mat4 matCam = glm::translate( m_robotState.position ) * glm::rotate( glm::radians( m_robotState.rotation ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
        glm::vec3 camPos = glm::vec3( matCam * glm::vec4(camPosInObjSpace,1.0f ) ); 

		m_camera.SetView(
			camPos,  // honnan nézzük a színteret  - eye
			m_robotState.position,  // a színtér melyik pontját nézzük  - at
			glm::vec3( 0.0f, 1.0f, 0.0f ) ); // felfelé mutató irány a világban  - up
	}
	else
	{
		m_cameraManipulator.Update(updateInfo.DeltaTimeInSec);
	}

    // Robot transzformációk 

    m_robotTransform = 
		glm::translate( m_robotState.position ) * 
		glm::rotate( glm::radians( m_robotState.rotation ), glm::vec3( 0, 1, 0 ) );

	m_robotBaseTransform = 
		m_robotTransform * 
		glm::translate(TORSO_POS);
	m_robotHeadTransform = 
		m_robotBaseTransform * 
		glm::translate(RELATIVE_HEAD_POS) *
		glm::rotate(m_robotHeadLeftRightRot, glm::vec3(0, -1, 0)) *
		glm::rotate(m_robotHeadUpDownRot, glm::vec3(1, 0, 0));
	m_robotLeftLegTransform =
		m_robotBaseTransform *
		glm::translate(RELATIVE_LEFT_LEG_POS) *
		glm::translate(CalculateLegAnim(m_robotState.legTime, 0.0f));
	m_robotRightLegTransform =
		m_robotBaseTransform *
		glm::translate(RELATIVE_RIGHT_LEG_POS) *
		glm::translate(CalculateLegAnim(m_robotState.legTime, glm::pi<float>()));
	m_robotLeftArmUpperTransform = 
		m_robotBaseTransform * 
		glm::translate(RELATIVE_LEFT_ARM_UPPER) * 
		glm::rotate(m_robotLeftArmUpperRot, glm::vec3(-1, 0, 0));
	m_robotRightArmUpperTransform = 
		m_robotBaseTransform * 
		glm::translate(RELATIVE_RIGHT_ARM_UPPER) * 
		glm::rotate(m_robotRightArmUpperRot, glm::vec3(-1, 0, 0));
	m_robotLeftArmLowerTransform = 
		m_robotLeftArmUpperTransform * // !!
		glm::translate(RELATIVE_LEFT_ARM_LOWER) * 
		glm::rotate(m_robotLeftArmLowerRot, glm::vec3(-1, 0, 0));
	m_robotRightArmLowerTransform = 
		m_robotRightArmUpperTransform * // !!
		glm::translate(RELATIVE_RIGHT_ARM_LOWER) * 
		glm::rotate(m_robotRightArmLowerRot, glm::vec3(-1, 0, 0));

	// robotból induló sugár metszése az asztallal 
	Ray headRay = CalculateRobotHeadRay();
	glm::vec3 intersectionPoint;
	if (intersectTableTop(headRay, intersectionPoint)) {
		m_lightPosition2 = glm::vec4(intersectionPoint + glm::vec3(0, 1.0f, 0), 1.0f);
	}
	else {
		m_lightPosition2 = glm::vec4(0, 1, 0, 0);
	}
}

void CMyApp::SetCommonUniforms()
{
	// - Uniform paraméterek 

	// - view és projekciós mátrix 
	glUniformMatrix4fv( ul("viewProj"), 1, GL_FALSE, glm::value_ptr(m_camera.GetViewProj()));

	// - Fényforrások beállítása 
	glUniform3fv( ul("cameraPosition"), 1, glm::value_ptr(m_camera.GetEye()));
	glUniform4fv( ul("lightPosition"), 1, glm::value_ptr(m_lightPosition));

	glUniform3fv( ul("La"), 1, glm::value_ptr(m_La));
	glUniform3fv( ul("Ld"), 1, glm::value_ptr(m_Ld));
	glUniform3fv( ul("Ls"), 1, glm::value_ptr(m_Ls));

	if ( m_lightPosition2.w != 0.0f )
	{
		glUniform4fv( ul("lightPosition2"), 1, glm::value_ptr( m_lightPosition2 ) );
		glUniform3fv( ul("La2"), 1, glm::value_ptr( m_La2 ) );
		glUniform3fv( ul("Ld2"), 1, glm::value_ptr( m_Ld2 ) );
		glUniform3fv( ul("Ls2"), 1, glm::value_ptr( m_Ls2 ) );
	}
	else
	{
		glUniform3f( ul("La2"), 0, 0, 0 );
		glUniform3f( ul("Ld2"), 0, 0, 0 );
		glUniform3f( ul("Ls2"), 0, 0, 0 );
	}

	glUniform1f( ul("lightConstantAttenuation"), m_lightConstantAttenuation);
	glUniform1f( ul("lightLinearAttenuation"), m_lightLinearAttenuation);
	glUniform1f( ul("lightQuadraticAttenuation"), m_lightQuadraticAttenuation);
}

void CMyApp::DrawObject(OGLObject& obj, const glm::mat4& world) {
	glUniformMatrix4fv( ul("world"), 1, GL_FALSE, glm::value_ptr(world));
	glUniformMatrix4fv( ul("worldInvTransp"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(world))));
	glBindVertexArray(obj.vaoID);
	glDrawElements(GL_TRIANGLES, obj.count, GL_UNSIGNED_INT, nullptr);
}

void CMyApp::CalculateRobotPosition(float deltaTime)
{
    //  ha a robot áll, akkor nem kell számolni semmit 
	// 
	if (m_robotState.state == RobotState::ROBOT_STATE_STAND) { return; }

    //  ha a robot mozog vagy forog, akkor számoljuk a láb animációhoz szükséges időt 
	// 
	m_robotState.legTime += deltaTime;

    //  az olvashatóságért használjunk referencia változókat a robot állapotának gyakran használt részeihez
    // 
	glm::vec3& pos = m_robotState.position;
    float& rotation = m_robotState.rotation;
	

    glm::vec3 desiredDiff = m_robotState.desiredPosition - pos; //  a kívánt pozíció és a jelenlegi pozíció különbsége... 
    float desiredDistance = glm::length( desiredDiff ); //  ...és a távolságuk 

    //  a robot előre néző iránya a jelenlegi forgás alapján 
    glm::vec3 forward = glm::vec3( sinf( glm::radians( rotation ) ), 0.0f, cosf( glm::radians( rotation ) ) );

	switch ( m_robotState.state )
	{
	case RobotState::ROBOT_STATE_ROTATING:	// forgatás 
	{
        glm::vec3 desiredDir = desiredDiff / desiredDistance; //  a kívánt irányba néző egységvektor 
        //  a kívánt irány és a jelenlegi előre néző irány közötti szög kiszámítása a vektoriális és skaláris szorzat segítségével (atan2f)
		// 
		
		float desiredDeltaSinRotation = glm::cross( forward, desiredDir ).y; 
		float desiredDeltaCosRotation = glm::dot( forward, desiredDir );

        //  a kívánt forgás 
		float desiredDeltaRotation = glm::degrees( ::atan2f( desiredDeltaSinRotation, desiredDeltaCosRotation ) );

        //  a maximális forgás, amit megengedünk ebben a frame-ben 
		float deltaRotation = m_robotState.rotationSpeed * deltaTime;

        if ( deltaRotation < fabsf( desiredDeltaRotation ) ) 
		{
			//  ha a kívánt forgás nagyobb, mint a megengedett maximális forgás, akkor csak a megengedett maximális forgást hajtjuk végre...
            // 
            deltaRotation = copysignf( deltaRotation, desiredDeltaRotation ); //  ...de megőrizzük a forgás irányát (balra vagy jobbra) 
			rotation += deltaRotation;
		}
		else
		{
            //  ha a kívánt forgás kisebb vagy egyenlő, mint a megengedett maximális forgás, akkor a kívánt forgást hajtjuk végre,...
			// 
			rotation += desiredDeltaRotation;
            m_robotState.state = RobotState::ROBOT_STATE_MOVING; //  ...és átállunk mozgás állapotba 
		}
	} break;

	case RobotState::ROBOT_STATE_MOVING:	// mozgás 
	{
        //  a kívánt irányba való mozgás vektora egy másodperc alatt a robot aktuális sebességével
		// 
		glm::vec3 movingDiffPerSec = forward * m_robotState.movementSpeed;

		//  a ténylegesen megtett mozgás vektora ebben a frame-ben
        // 
		glm::vec3 movingVector = movingDiffPerSec * deltaTime;

        pos += movingVector; //  a robot pozíciójának frissítése a mozgás vektorral 

		//  ha a kívánt pozíció közelebb van, mint a ténylegesen megtett mozgás hossza, ...
        // 
		if ( desiredDistance <= glm::length( movingVector ) ) 
		{
			pos = m_robotState.desiredPosition; //  ...akkor a robotot a kívánt pozícióra helyezzük, ... 
            m_robotState.state = RobotState::ROBOT_STATE_STAND; //  ...és átállunk álló állapotba 
		}
	}break;
	}
}

Ray CMyApp::CalculateRobotHeadRay() const
{
	Ray headRay;
    headRay.origin = glm::vec3( m_robotHeadTransform * glm::vec4( AVG_EYE_POS_OFFSET, 1 ) );
    headRay.direction = glm::vec3( m_robotHeadTransform * glm::vec4( 0, 0, 1, 0 ) );

	return headRay;
}

void CMyApp::RenderRobot()
{
	// Textúra 
	glBindTextureUnit(0, m_robotTextureID);
	glBindTextureUnit(1, m_shineTextureID);
	glBindSampler(0, m_SamplerID);
	glBindSampler(1, m_SamplerID);
	glUniform1i( ul("textureImage"), 0);
	glUniform1i( ul("textureShinning"), 1);
	// - Anyagjellemzők beállítása 
	glUniform3fv( ul("Ka"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
	glUniform3fv( ul("Kd"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
	glUniform3fv( ul("Ks"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
	// state - rajzolási mód 
	glUniform1i( ul("state"), SHADER_STATE_ROBOT);

	DrawObject( m_robotTorsoGPU, m_robotBaseTransform );

	DrawObject( m_robotHeadGPU, m_robotHeadTransform );
    DrawObject( m_robotLegGPU, m_robotLeftLegTransform );
    DrawObject( m_robotLegGPU, m_robotRightLegTransform );
	DrawObject( m_robotLeftArmUpperGPU, m_robotLeftArmUpperTransform );
    DrawObject( m_robotRightArmUpperGPU, m_robotRightArmUpperTransform );
	DrawObject( m_robotLeftArmLowerGPU, m_robotLeftArmLowerTransform );
	DrawObject( m_robotRightArmLowerGPU, m_robotRightArmLowerTransform );
}

void CMyApp::RenderTable()
{
	// - Anyagjellemzők beállítása 
	glUniform3fv( ul("Ka"), 1, glm::value_ptr(m_Ka));
	glUniform3fv( ul("Kd"), 1, glm::value_ptr(m_Kd));
	glUniform3fv( ul("Ks"), 1, glm::value_ptr(m_Ks));

	glUniform1f( ul("Shininess"), m_Shininess);

	// - textúraegységek beállítása 
	glUniform1i( ul("textureImage"), 0);

	// - Textúrák beállítása, minden egységre külön 
	glBindTextureUnit(0, m_woodTextureID);
	glBindSampler(0, m_SamplerID);
	
    // Asztal állapotának beállítása 
	glUniform1i( ul("state"), SHADER_STATE_TABLE);

	glm::mat4 tableTransform = glm::translate(TABLE_POS);

	// Rajzolási parancs kiadása 
	DrawObject(m_cubeGPU, tableTransform * glm::scale(TABLE_SIZE));

	glUniform1i( ul("state"), SHADER_STATE_DEFAULT);
	
	for (int i = -1; i <= 1; i += 2) {
		for (int j = -1; j <= 1; j += 2) {
			glm::vec3 legPos = glm::vec3(i * 60, -(TABLE_LEG_SIZE.y + TABLE_SIZE.y) / 2.f, j * 60);
			DrawObject(m_cubeGPU, tableTransform * glm::translate(legPos) * glm::scale(TABLE_LEG_SIZE));
		}
	}
}

void CMyApp::Render()
{
	// töröljük a framepuffert (GL_COLOR_BUFFER_BIT)...
	// ... és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_programID);

	SetCommonUniforms();

	glUniform1i( ul("debugState"), m_shaderDebugState);

	RenderRobot();
	RenderTable();

	// debug kocka, a robot célja 
	if (m_drawRobotGoal) {
		glUniform1i( ul("state"), SHADER_STATE_DEFAULT);
		glm::mat4 goalWorldTr = glm::translate(m_robotState.desiredPosition) * glm::scale(glm::vec3(m_robotCubeGoalSize));
		DrawObject(m_cubeGPU, goalWorldTr);
	}


	glBindVertexArray(0);
	// shader kikapcsolása 
	glUseProgram(0);
}

void CMyApp::CalculateRobotMovementData(glm::vec3 desiredPosition)
{
	m_robotState.desiredPosition = desiredPosition;
	m_robotState.state = RobotState::ROBOT_STATE_ROTATING;
}

void CMyApp::RenderGUI()
{
	//ImGui::ShowDemoWindow();

	if (ImGui::Begin("Robot")) {
		ImGui::SeparatorText("Animations");
		ImGui::SliderAngle("Left Upper Arm", &m_robotLeftArmUpperRot, -90.0f, 180.0f);
		ImGui::SliderAngle("Left Lower Arm", &m_robotLeftArmLowerRot, -120.0f, 120.0f);
		ImGui::SliderAngle("Right Upper Arm", &m_robotRightArmUpperRot, -90.0f, 180.0f);
		ImGui::SliderAngle("Right Lower Arm", &m_robotRightArmLowerRot, -120.0f, 120.0f);
		ImGui::SliderAngle("Head Up-Down", &m_robotHeadUpDownRot, -20.0f, 50.0f);
		ImGui::SliderAngle("Head Left-Right", &m_robotHeadLeftRightRot, -120.0f, 120.0f);

		ImGui::SeparatorText("Shader Debug");
		ImGui::Combo("Debug State", &m_shaderDebugState, "None\0Normals\0Shading\0Diffuse texture\0Shine texture\0");

		ImGui::SeparatorText("3rd Prerson View");
		ImGui::Checkbox("3rd Person View Camera", &m_isTopCamera);

		ImGui::SeparatorText("Robot goal draw");
		ImGui::Checkbox("Draw robot goal", &m_drawRobotGoal);
		ImGui::SliderFloat("Goal cube size", &m_robotCubeGoalSize, 1.0f, 100.f);

		ImGui::SeparatorText("Robot moving speeds");
        ImGui::SliderFloat( "Movement speed", &m_robotState.movementSpeed, 1.0f, 100.0f );
        ImGui::SliderFloat( "Rotation speed", &m_robotState.rotationSpeed, 1.0f, 360.0f );

	}
	ImGui::End();
}

// https://wiki.libsdl.org/SDL2/SDL_KeyboardEvent
// https://wiki.libsdl.org/SDL2/SDL_Keysym
// https://wiki.libsdl.org/SDL2/SDL_Keycode
// https://wiki.libsdl.org/SDL2/SDL_Keymod

void CMyApp::KeyboardDown(const SDL_KeyboardEvent& key)
{
	if (!key.repeat) // Először lett megnyomva 
	{
		if (key.key == SDLK_F5 && key.mod & SDL_KMOD_CTRL)
		{
			CleanShaders();
			InitShaders();
		}
		if (key.key == SDLK_F1)
		{
			GLint polygonModeFrontAndBack[2] = {};
			// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGet.xhtml
			glGetIntegerv( GL_POLYGON_MODE, polygonModeFrontAndBack ); // Kérdezzük le a jelenlegi polygon módot! Külön adja a front és back módokat. 
			GLenum polygonMode = ( polygonModeFrontAndBack[ 0 ] != GL_FILL ? GL_FILL : GL_LINE ); // Váltogassuk FILL és LINE között! 
			// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glPolygonMode.xhtml
			glPolygonMode( GL_FRONT_AND_BACK, polygonMode ); // Állítsuk be az újat! 
		}

		if (key.key == SDLK_LCTRL || key.key == SDLK_RCTRL)
		{
			m_IsCtrlDown = true;
		}
	}
	m_cameraManipulator.KeyboardDown(key);
}

void CMyApp::KeyboardUp(const SDL_KeyboardEvent& key)
{
	m_cameraManipulator.KeyboardUp(key);
	if (key.key == SDLK_LCTRL || key.key == SDLK_RCTRL)
	{
		m_IsCtrlDown = false;
	}
}

// https://wiki.libsdl.org/SDL2/SDL_MouseMotionEvent

void CMyApp::MouseMove(const SDL_MouseMotionEvent& mouse)
{
	m_cameraManipulator.MouseMove(mouse);
}

// https://wiki.libsdl.org/SDL2/SDL_MouseButtonEvent

void CMyApp::MouseDown(const SDL_MouseButtonEvent& mouse)
{
	if ( m_IsCtrlDown )
	{
		m_IsPicking = true;
	}
	m_PickedPixel = { mouse.x, mouse.y };
}

void CMyApp::MouseUp(const SDL_MouseButtonEvent& mouse)
{
}

// https://wiki.libsdl.org/SDL2/SDL_MouseWheelEvent

void CMyApp::MouseWheel(const SDL_MouseWheelEvent& wheel)
{
	m_cameraManipulator.MouseWheel(wheel);
}

// a két paraméterben az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);
	m_windowSize = glm::uvec2(_w, _h);
	m_camera.SetAspect(static_cast<float>(_w) / _h);
}

// Le nem kezelt, egzotikus esemény kezelése
// https://wiki.libsdl.org/SDL2/SDL_Event

void CMyApp::OtherEvent(const SDL_Event& ev)
{

}