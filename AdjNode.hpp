#pragma once
class AdjNode
{
public:
	AdjNode(void);
	AdjNode(int,int);
	~AdjNode(void);
	int GetLength();
	void SetLength(int);
	int GetNext();
	void SetNext(int);

private:
	int length;
	int next;
};

