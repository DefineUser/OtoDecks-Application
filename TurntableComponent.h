/*
  ==============================================================================

    TurntableComponent.h
    Created: 18 Aug 2023 5:51:30pm
    Author:  James

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//custom listener class for angleDifference
class AngleChangeListener
{
public:
    virtual ~AngleChangeListener() = default; //destructor
    virtual void angleChanged(float angleDifference) = 0; //pure virtual
};

class TurntableComponent : public Component, public MouseListener
{
public:
    TurntableComponent();
    ~TurntableComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    //functions for rotating the disc
    void rotationUpdate(float angleUpdate);
    void resetRotation();

    //functions for disc to control playback of audio tracks
    float angleFromCenter(const Point<float>& point);
    bool isMouseWithinDisc(const Point<float>& point);
    void mouseDown(const MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;

    void addAngleChangeListener(AngleChangeListener* listener);
    void removeAngleChangeListener(AngleChangeListener* listener);

private:
    float angle, position, diameter, radius, discCenterX, discCenterY, startAngle;
    Point<float> startPoint;
    Array<AngleChangeListener*> angleChangeListeners;

    void broadcastAngleChange(float angleDifference);

    float getWidthFloat() const { return static_cast<float>(getWidth()); }
    float getHeightFloat() const { return static_cast<float>(getHeight()); }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TurntableComponent)
};
