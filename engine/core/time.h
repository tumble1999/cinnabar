#ifndef _CE_TIME_H_
#define _CE_TIME_H_

namespace ce {
class Time
{
private:
	float 
		m_now,
		m_last,
		m_dt,
		m_fps;

	float getTime();
public:
	Time();
	~Time();

	void update();

	float getDeltaTime() {return m_dt;}
	float getFPS() {return m_fps;};
};
}

#endif // !_CE_TIME_H_