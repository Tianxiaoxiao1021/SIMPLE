//Scontrol.h
#ifndef SCONTROL_H
#define SCONTROL_H
#include"dependency.h"

// �ؼ�����
class Scontrol {
private:
	bool visible;
	int width, height;
	int posX, posY;
	Swindow* parent;
	Scolor bkColor;
public:
	// 构造函数
	Scontrol(Swindow* p, int w, int h) 
	:visible(true), width(w), height(h), parent(p), posX(0), posY(0) {
	}
	Scontrol(Swindow* p, int w, int h,int x,int y)
	:visible(true), width(w), height(h), parent(p), posX(x), posY(y) {
	}
	Scontrol(Swindow* p, int w, int h,int x,int y,Scolor c)
	:visible(true), width(w), height(h), parent(p), posX(x), posY(y),bkColor(c) {
	}
	// getter & setter
	void setBkColor(Scolor c) {this->bkColor = c;}
	Scolor getBkColor() const {return this->bkColor;}
	void setVisible(bool v) {this->visible = v;}
	bool isVisible() const {return this->visible;}
	int getWidth() const {return this->width;}
	int getHeight() const {return this->height;}
	void setParent(Swindow* p) {this->parent = p;}
	Swindow* getParent() const {return this->parent;}
	void setSize(int w, int h) {
		this->width = w;
		this->height = h;
	}
	void setPos(int x, int y) {
		this->posX = x;
		this->posY = y;
	}
	std::pair<int, int> getPos() const {return std::make_pair(posX, posY);}

	void render();
};
// �б���
class Slist : public Scontrol {
private:
	int size;
	std::vector<SlistItem*> items;
	std::vector<Slist*> sublists;
public:
	Slist(Swindow* p, int w, int h)
		:Scontrol(p,w,h),size(0) {
	}
	Slist(Swindow* p, int w, int h)
		:Scontrol(p,w,h),size(0) {
	}
	std::vector<SlistItem*>getItems() const { return items; }
	void addSublist(Slist* s) { if (s && s->isVisible() && s != this)sublists.push_back(s); size++; }
	void deleteSublist(int index) {
		if (index >= 0 && index < sublists.size()) {
			delete sublists[index];
			sublists.erase(sublists.begin() + index);
		}
	}
	std::vector<Slist*>getSublists() const { return sublists; }
	void addItem(SlistItem* i);
	void deleteItem(int index) {
		if (index >= 0 && index < items.size()) {
			delete items[index];
			items.erase(items.begin() + index);
		}
	}
};
// �հ׿���
class SemptyBox : public Scontrol {
private:
	std::vector<Scontrol*> controls;
public:
	SemptyBox(Swindow* p, int w, int h)
		:Scontrol(p, w, h) {}
	SemptyBox(Swindow* p, int w, int h)
		:Scontrol(p, w, h) {}
	void addControl(Scontrol* c) {
		if(c && c->isVisible() && c!= this)controls.push_back(c);
	}
	void deleteControl(int index) {
		if (index >= 0 && index < controls.size()) {
			delete controls[index];
			controls.erase(controls.begin() + index);
		}
	}
	std::vector<Scontrol*>getControls() const { return controls; }
};
class Slabel : public Scontrol {
private:
	std::string text;
public:
	Slabel(Swindow* p, int w, int h)
		:Scontrol(p, w, h) {}
	Slabel(Swindow* p, int w, int h)
		:Scontrol(p, w, h) {}
	Slabel(Swindow* p, int w, int h, std::string t)
		:Scontrol(p, w, h), text(t) {}
	void setText(std::string t) {
		this->text = t;
	}
	std::string getText() const {
		return this->text;
	}
};
// �ַ�������ɫ�Ľṹ��
struct StextElement {
	char c;        // �ַ�
	Scolor color;  // ��Ӧ����ɫ
};
// �� SeditBox ��ʹ�õ��ַ���
struct Stext {
	std::vector<StextElement> elements;  // �洢�ַ�����ɫ������
	size_t size() const {return elements.size();}
	char getChar(size_t index) const {
		if (index < elements.size()) {
			return elements[index].c;
		}
		return '\0';
	}
	Scolor getColor(size_t index) const {
		if (index < elements.size()) {
			return elements[index].color;
		}
		return Scolor::BLACK;
	}
	void setColor(size_t index, Scolor color) {
		if (index < elements.size()) {
			elements[index].color = color;
		}
	}
	void addChar(char c, Scolor color) {
		elements.push_back({ c, color });
	}
	void deleteChar() {
		if (!elements.empty()) {
			elements.pop_back();
		}
	}
	void clear() {
		elements.clear();
	}
};
class SeditBox : public SemptyBox {
private:
	Stext m_Text;
	int cursorX, cursorY;// 光标位置
	Scolor defaultColor;// 默认字符颜色
public:
	SeditBox(Swindow* p, int w, int h)
		: SemptyBox(p, w, h), cursorX(0), cursorY(0){}

	SeditBox(Swindow* p, int w, int h)
		: SemptyBox(p, w, h), cursorX(0), cursorY(0) {}

	SeditBox(Swindow* p, int w, int h, std::string t)
		: SemptyBox(p, w, h), cursorX(0), cursorY(0) {
		for (char c : t) {
			m_Text.addChar(c, Scolor::BLACK);
		}
	}
	void setCursorPos(int x, int y) {
		this->cursorX = x;
		this->cursorY = y;
	}
	std::pair<int, int> getCursorPos() const {return std::make_pair(cursorX, cursorY);}
	bool find(char c){
		int l, r;
		l = 0;
		r = m_Text.size() - 1;
		while (l <= r) {
			int mid = (l + r) / 2;
			if (m_Text.getChar(mid) == c)return true;
			else if (m_Text.getChar(mid) < c)l=mid+1;
			else r=mid-1;
		}
	}
	void setCharColor(size_t index, Scolor color) {m_Text.setColor(index, color);}
	Scolor getCharColor(size_t index) const {return m_Text.getColor(index);}
	void setTextColor(Scolor color) {
		for (size_t i = 0; i < m_Text.size(); ++i) {
			m_Text.setColor(i, color);
		}
	}
	void setText(std::string t) {
		m_Text.clear();
		for (char c : t) {
			m_Text.addChar(c, Scolor::BLACK);
		}
	}
	Stext getText() const {return m_Text;}
	void appendText(std::string t) {
		for (char c : t) {
			m_Text.addChar(c, Scolor::BLACK);
		}
	}
	void addChar(char c) {m_Text.addChar(c, Scolor::BLACK);}
	void addChar(char c, Scolor color) {m_Text.addChar(c, color);}
	void deleteChar() {m_Text.deleteChar();}
	void deleteChar(size_t index) {
		if (index < m_Text.size()) {
			m_Text.elements.erase(m_Text.elements.begin() + index);
		}
	}
	void clearText() {m_Text.clear();}
	void insertText(std::string t) {
		for (char c : t) {
			m_Text.addChar(c, Scolor::BLACK);
		}
	}
	void insertText(std::string t,Scolor cColor) {
		for (char c : t) {
			m_Text.addChar(c, cColor);
		}
	}
	void insertText(std::string t, size_t index) {
		if (index < m_Text.size()) {
			for (char c : t) {
				m_Text.elements.insert(m_Text.elements.begin() + index, { c, Scolor::BLACK });
				++index;
			}
		}
	}
	void insertText(std::string t, size_t index,Scolor cColor) {
		if (index < m_Text.size()) {
			for (char c : t) {
				m_Text.elements.insert(m_Text.elements.begin() + index, { c, cColor });
				++index;
			}
		}
	}

	void OnTextChange();
};
// ��ť��
class Sbutton : public Scontrol {
private:
	Stext text;
	bool pressed;
	Scolor normalColor, pressedColor, currColor;
public:
	Sbutton(Swindow* p, int w, int h)
		:Scontrol(p, w, h), pressed(false) {
		this->normalColor = Scolor::WHITE;
		this->pressedColor = Scolor::GRAY;
		this->currColor = this->normalColor;
	}
	Sbutton(Swindow* p, int w, int h)
		:Scontrol(p, w, h), pressed(false) {
		this->normalColor = Scolor::WHITE;
		this->pressedColor = Scolor::GRAY;
		this->currColor = this->normalColor;
	}
	Sbutton(Swindow* p, int w, int h, Stext t, Scolor c1, Scolor c2)
		:Scontrol(p, w, h), text(t), pressed(false), normalColor(c1), pressedColor(c2) {
		this->currColor = this->normalColor;
	}
	void setText(Stext t) {
		this->text = t;
	}
	Stext getText() const {
		return this->text;
	}
	bool isPressed() const {
		return this->pressed;
	}
	void OnPress();
	void OnRelease();
};
#endif