#pragma once

class DateInfo {
private:
	int year;
	int month;
	int day;

public:
	DateInfo();
	DateInfo(int year, int month, int day);
	void SetDay(int day);
	int GetDay();
	void SetYear(int year);
	int GetYear();
	void SetMonth(int month);
	int GetMonth();
};

class TimeInfo :public DateInfo
{
private:
	int hour;
	int minute;
	int second;

public:
	TimeInfo();
	TimeInfo(int hour, int minute, int second);
	void SetHour(int hour);
	int GetHour();
	void SetMinute(int minute);
	int GetMinute();
	void SetSecond(int second);
	int GetSecond();

};

class MyString
{
private:
	char* str;
public:
	MyString(int length);
	MyString();
	~MyString();
	void SetString(char* string);
	void PrintString();
	void AppendString(char* string);
	int size();
};