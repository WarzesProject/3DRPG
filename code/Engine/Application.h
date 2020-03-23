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
				isExit = IsErrorCritical();
#endif
				if ( !isExit && userApp.Init() )
				{
#pragma warning(disable: 4626)
#if SE_ENABLE_EXCEPTION
					auto funcInMainLoop = [&isExit](bool retFunc) -> void { isExit = isExit || !retFunc; };
#else
					auto funcInMainLoop = [&isExit](bool retFunc) -> void { isExit = (isExit || !(!IsErrorCritical() && retFunc)); };
#endif
#pragma warning(default: 4626)

					while ( !isExit )
					{
						engineApp.deltaTime();

						// Event Update
						funcInMainLoop(engineApp.update());
						funcInMainLoop(userApp.Update());

						// Render Draw
						funcInMainLoop(engineApp.beginFrame());
						funcInMainLoop(userApp.Frame());
						funcInMainLoop(engineApp.endFrame());

						// exit command
						isExit = isExit || Application::m_isExitCommand;
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