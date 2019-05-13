#include "allheader.h" 
#include "shader.h"
//#include "camera.h"
#include "Vertex.h"

//异常回调函数
static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

//顶点着色器
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

//片段着色器
const char *fragmentShaderSource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color;\n"
"void main()\n"
"{\n"
"   FragColor = color;\n"
"}\n\0";


//立方体的六个面->每个面由两个三角形组成，每个面6个顶点

int main(int, char**)
{
	//-------------------------------------------------------------
	// Setup window、设置异常回调函数
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
	// 声明一个窗口对象

	GLFWwindow* window = glfwCreateWindow(MAXWIDTH,MAXHEIGHT, "OpenGL3", NULL, NULL);
	if (window == NULL)
		return 1;

	//为窗口对象创建一个乡下文
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
	//设置回调
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);

	//---------------------------------------------------------------
	// 初始化opengl函数加载器
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
	//是否开启深度测试
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
	//生成着色器程序:从上到下依次为Phone shading\Gouraud shading\Light resource的shader
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
	//菜单选项的值
	bool perspective = false;
	
	//设定平面的顶点缓冲plainVBO
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

	//设置立方体的顶点缓冲cubeVBO
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

	//创建帧缓冲对象
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	//创建对应的纹理对对象作为帧缓冲的附件
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	//设置纹理的相关参数：纹理映射、环绕方式、数据等
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, MAXWIDTH, MAXHEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//将纹理对象添加到帧缓冲中
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// Main loop
	while (!glfwWindowShouldClose(window))//判定窗口是否关闭，依次来进行循环渲染
	{
		
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//以下是创建一个图形交互界面
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
		//调节视口，让其与窗口大小保存一致
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		//清屏，以此来设置背景色
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//每次循环刷新图形界面的帧
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/*
			通过第一步的渲染，将在光源视角下的渲染的深度信息写入到指定的帧缓冲当中
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
			此时已经得到了在光源视角下的scene的深度信息
		*/


		/*
			此时可以借助得到的深度信息来进行阴影的渲染了
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
		//传入观察位置
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
			阴影渲染结束
		*/
		
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	// Cleanup
	//解除ImGUI与opengl、glfw之间的关联资源
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	//释放imGUI的相关资源
	ImGui::DestroyContext();
	//删除前面所生成的VAO、VBO
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &cubeVBO);
	//释放窗口对象
	glfwDestroyWindow(window);
	//释放所有的窗口资源
	glfwTerminate();

	return 0;
}
