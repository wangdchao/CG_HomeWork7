#include "allheader.h" 
#include "shader.h"
//#include "camera.h"
#include "Vertex.h"

//�쳣�ص�����
static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

//������ɫ��
const char *vertexShaderSource1 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 pers;\n"
"void main()\n"
"{\n"
"	vec4 dot= vec4(aPos.x,aPos.y, aPos.z, 1.0f);\n"	
"   gl_Position = pers*view*model*dot; \n"
"}\0";

//Ƭ����ɫ��
const char *fragmentShaderSource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color;\n"
"void main()\n"
"{\n"
"   FragColor = color;\n"
"}\n\0";


//�������������->ÿ������������������ɣ�ÿ����6������

int main(int, char**)
{
	//-------------------------------------------------------------
	// Setup window�������쳣�ص�����
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	//-------------------------------------------------------------
	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
	

	//-----------------------------------------------------------------------------
	// ����һ�����ڶ���

	GLFWwindow* window = glfwCreateWindow(MAXWIDTH,MAXHEIGHT, "OpenGL3", NULL, NULL);
	if (window == NULL)
		return 1;

	//Ϊ���ڶ��󴴽�һ��������
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
	//���ûص�
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);

	//---------------------------------------------------------------
	// ��ʼ��opengl����������
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
	bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
	bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
	bool err = gladLoadGL() == 0;
#else
	bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}
	//�Ƿ�����Ȳ���
	if (ENABLE) glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, MAXWIDTH, MAXHEIGHT); 
	//-----------------------------------------------------------------
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	int a;
	//-------------------------------------------------------------------
	//������ɫ������:���ϵ�������ΪPhone shading\Gouraud shading\Light resource��shader
	shader getDepthShader = shader();
	if (!getDepthShader.createShaderWithFile("shadow_getDepth_VS.txt", "shadow_getDepth_FS.txt")) {
		printf("Error when create cubeshader\n");
		std::cin >> a;
		exit(0);
	}
	shader applyDepthShader = shader();
	if (!applyDepthShader.createShaderWithFile("shadow_applyDepth_VS.txt", "shadow_applyDepth_FS.txt")) {
		printf("Error when create depthshader\n");
		std::cin >> a;
		exit(0);
	}
	glUniform1i(glGetUniformLocation(applyDepthShader.getID(), "shadowMap"), 0);
	/*shader plainshader = shader();
	if (!plainshader.createShaderWithFile("..txt", "..txt")) {
		printf("Error when create plainshader\n");
		exit(0);
	}*/
	//--------------------------------------------------------------------
	//�˵�ѡ���ֵ
	bool perspective = false;
	
	//�趨ƽ��Ķ��㻺��plainVBO
	GLuint planeVAO;
	GLuint planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	//����������Ķ��㻺��cubeVBO
	GLuint cubeVAO;
	GLuint cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	float x = -2.0, y = 4.0, z = -1.0;
	glm::vec3 lightPos(x, y, z);

	//����֡�������
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	//������Ӧ������Զ�����Ϊ֡����ĸ���
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	//�����������ز���������ӳ�䡢���Ʒ�ʽ�����ݵ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, MAXWIDTH, MAXHEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//�����������ӵ�֡������
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// Main loop
	while (!glfwWindowShouldClose(window))//�ж������Ƿ�رգ�����������ѭ����Ⱦ
	{
		
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//�����Ǵ���һ��ͼ�ν�������
		ImGui::Begin("Light and Shading!", NULL, ImGuiWindowFlags_MenuBar);
		ImGui::Checkbox("Perspective", &perspective);
		ImGui::SliderFloat("light pos X", &x ,-5.0,5.0 );
		ImGui::SliderFloat("light pos Y", &y ,0.0,5.0 );
		ImGui::SliderFloat("light pos Z", &z ,-5.0,5.0 );
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
		
		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		//�����ӿڣ������봰�ڴ�С����һ��
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		//�������Դ������ñ���ɫ
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//ÿ��ѭ��ˢ��ͼ�ν����֡
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/*
			ͨ����һ������Ⱦ�����ڹ�Դ�ӽ��µ���Ⱦ�������Ϣд�뵽ָ����֡���嵱��
			-------------------------- start -------------------------------------
		*/
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		if (perspective) lightProjection = glm::perspective(glm::radians(45.0f), (float)MAXWIDTH / (float)MAXHEIGHT, 0.1f, 7.5f);
		else lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
		lightView = glm::lookAt(lightPos=glm::vec3(x,y,z), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		glm::mat4 model = glm::mat4(1.0f);

		glViewport(0, 0, MAXWIDTH, MAXHEIGHT);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		getDepthShader.use();
		getDepthShader.setVertexUniformView(lightSpaceMatrix, "lightSpaceMatrix");
		getDepthShader.setVertexUniformModel(model, "model");
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0));
		getDepthShader.setVertexUniformModel(model, "model");
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0,(GLuint)36);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		/*
			------------------------------End------------------------------------
			��ʱ�Ѿ��õ����ڹ�Դ�ӽ��µ�scene�������Ϣ
		*/


		/*
			��ʱ���Խ����õ��������Ϣ��������Ӱ����Ⱦ��
			-----------------------------Start-----------------------------------
		*/
		glViewport(0, 0, MAXWIDTH, MAXHEIGHT);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		applyDepthShader.use();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)MAXWIDTH / (float)MAXHEIGHT, 0.1f, 100.0f);
		glm::mat4 view1 = glm::mat4(1.0f);
		view1 = glm::rotate(view1, glm::radians(1.0f), glm::vec3(0.0, 1.0, 0.0));
		view1 = glm::translate(view1, glm::vec3(0.0f, -2.0f, -10.0f));
		applyDepthShader.setVertexUniformModel(glm::mat4(1.0f), "model");
		applyDepthShader.setVertexUniformView(view1, "view");
		applyDepthShader.setVertexUniformPers(projection, "projection");
		applyDepthShader.setVertexUniformView(lightSpaceMatrix, "lightSpaceMatrix");
		applyDepthShader.setVec3("lightPos", lightPos);
		//����۲�λ��
		applyDepthShader.setVec3("viewPos", glm::vec3(0.0f,0.0f,0.0f));
		// Set light uniforms
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES,0, 6);
		glm::mat4 model1 = glm::mat4(1.0f);
		model1=translate(model1, glm::vec3(0.0f, 0.5f, 0.0));
		applyDepthShader.setVertexUniformModel(model1, "model");
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0,36);
		/*
			-----------------------------End-------------------------------------
			��Ӱ��Ⱦ����
		*/
		
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	// Cleanup
	//���ImGUI��opengl��glfw֮��Ĺ�����Դ
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	//�ͷ�imGUI�������Դ
	ImGui::DestroyContext();
	//ɾ��ǰ�������ɵ�VAO��VBO
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &cubeVBO);
	//�ͷŴ��ڶ���
	glfwDestroyWindow(window);
	//�ͷ����еĴ�����Դ
	glfwTerminate();

	return 0;
}
