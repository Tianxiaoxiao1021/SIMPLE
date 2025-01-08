//SsignalEngine.h
#ifndef SSIGNALENGINE_H
#define SSIGNALENGINE_H
#pragma once
#include "dependency.h"
// ���� SignalEngine ��
class SsignalEngine {
public:
    // ʹ�� unordered_map ��ӳ���ź��������Ӧ�Ĳۺ����б�
    std::unordered_map<std::string, std::vector<std::function<void(std::any)>>> signals;

    // �����źźͲ�
    void connect(const std::string& signalName, std::function<void(std::any)> slot) {
        signals[signalName].push_back(slot);  // ���ۺ�����ӵ���Ӧ�źŵĲۺ����б���
    }

    // �����ź�
    void emit(const std::string& signalName, std::any arg) {
        auto it = signals.find(signalName);
        if (it != signals.end()) {
            // �������źŶ�Ӧ�����вۺ���������
            for (auto& slot : it->second) {
                slot(arg);  // ִ���������ӵĲۺ����������ݲ���
            }
        }
    }
};
#endif