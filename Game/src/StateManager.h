#ifndef STATEMANAGER_H
#define STATEMANAGER_H

class StateManager
{
private:
	int mCurrentState;
	int mPreviousState;
public:
	StateManager();
	StateManager(int initialState);
	virtual ~StateManager();
	
	int GetCurrentState() const { return mCurrentState; }
	int GetPreviousState() const { return mPreviousState; }
	void SetCurrentState(int state) 
	{ 
		if(mCurrentState != state) 
		{
			mPreviousState = mCurrentState;
			mCurrentState = state;
		}
	}
};

#endif
