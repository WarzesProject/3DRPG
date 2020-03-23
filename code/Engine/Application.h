#pragma once

#include "Engine/Configuration.h"
#include "Engine/Error.h"
#include "Engine/Log.h"

class Application
{
public:
	template<typename AppImpl>
	static int Run(const Configuration &config)
	{
#if SE_ENABLE_EXCEPTION
		try
#endif
		{
			Application engineApp;
			if ( engineApp.init(config) )
			{
				AppImpl userApp;

				bool isExit = false;
#if !SE_ENABLE_EXCEPTION
				isExit = IsFatalError();
#endif
				if ( !isExit && userApp.Init() )
				{
					while ( !isExit )
					{
						engineApp.deltaTime();

#if SE_ENABLE_EXCEPTION
#	define FUNCINMAIN(func) { isExit = (isExit || !func); }
#else
#	define FUNCINMAIN(func) { isExit = (isExit || !(!IsFatalError() && func)); }
#endif
						// Event Update
						FUNCINMAIN(engineApp.update());
						FUNCINMAIN(userApp.Update());

						// exit command
						isExit = isExit || Application::m_isExitCommand;

						// Render Draw
						FUNCINMAIN(engineApp.beginFrame());
						FUNCINMAIN(userApp.Frame());
						FUNCINMAIN(engineApp.endFrame());
#undef FUNCINMAIN
					}
				}
			}			
		}
#if SE_ENABLE_EXCEPTION
		catch ( const Exception &exc )
		{
			SE_LOG_ERROR(exc.what());
		}
#endif
		return 0;
	}

	static void Exit()
	{
		m_isExitCommand = true;
	}

private:
	Application();
	Application(Application&&) = delete;
	Application(const Application&) = delete;
	Application& operator=(Application&&) = delete;
	Application& operator=(const Application&) = delete;
	~Application();

	bool init(const Configuration &config);
	bool initSubsystem();
	void deltaTime();
	bool beginFrame();
	bool endFrame();
	bool update();
	void close();

	static inline bool m_isExitCommand = false;

	Configuration m_config;
	struct AppPimpl;
	AppPimpl *m_pimpl = nullptr;
	
};