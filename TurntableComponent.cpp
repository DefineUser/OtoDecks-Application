/*
  ==============================================================================

    TurntableComponent.cpp
    Created: 18 Aug 2023 5:51:30pm
    Author:  James

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TurntableComponent.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846 //pi
#endif

//==============================================================================
TurntableComponent::TurntableComponent() : angle(0.0f),
                                           position(0.0f),
                                           startPoint(0.0f, 0.0f),
                                           diameter(0.0f),
                                           radius(0.0f),
                                           discCenterX(0.0f),
                                           discCenterY(0.0f),
                                           startAngle(0.0f)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

TurntableComponent::~TurntableComponent() {}

void TurntableComponent::paint (juce::Graphics& g)
{
    //specifics for drawing the disc
    diameter = getHeightFloat();
    radius = diameter / 2.0f;

    discCenterX = getWidthFloat() / 2.0f;
    discCenterY = getHeightFloat() / 2.0f;

    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId)); //clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1); //draw an outline around the component

    g.setColour(Colours::black);
    Rectangle<float> area {discCenterX - radius, 0.0f, diameter, diameter};
    g.fillEllipse(area);

    //drawing smaller inner ellipses around the main disc
    float innerDiameter = diameter / 3.5f;
    float innerRadius = innerDiameter / 2.0f;
    g.setColour(Colours::white);
    Rectangle<float> smallerArea {discCenterX - innerRadius, discCenterY - innerRadius, innerDiameter, innerDiameter};
    g.fillEllipse(smallerArea);

    //calculate the starting point of the line on the inner ellipse based on the angle
    Point<float> startPoint = Point<float>(discCenterX, discCenterY).getPointOnCircumference(innerDiameter / 2.0f, degreesToRadians(angle));

    //calculate the ending point of the line on the outer ellipse using the same angle
    Point<float> endPoint = Point<float>(discCenterX, discCenterY).getPointOnCircumference(radius, degreesToRadians(angle));

    //draw the line
    g.setColour(Colours::red); // or any other color you prefer
    g.drawLine(startPoint.getX(), startPoint.getY(), endPoint.getX(), endPoint.getY(), 10.0f);
}

void TurntableComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

//logic for disc rotation update
void TurntableComponent::rotationUpdate(float angleUpdate)
{
    //update angle from angleUpdate
    angle += angleUpdate;
    angle = fmod(angle, 360.0f); //(0, 360) range
    repaint();  //redraw
}

//reset the rotation to its default position
void TurntableComponent::resetRotation()
{
    angle = 0;
    repaint();  //redraw
}

//get angle of the starting point from the center of the disc
float TurntableComponent::angleFromCenter(const Point<float>& point)
{
    Point<float> delta = point - Point<float>(discCenterX, discCenterY);
    return atan2(delta.y, delta.x); //arctangent of 2 points, in radians
}

bool TurntableComponent::isMouseWithinDisc(const Point<float>& point)
{
    //returns point from center of disc and radius
    return point.getDistanceFrom(Point<float>(discCenterX, discCenterY)) <= radius;
}

void TurntableComponent::mouseDown(const MouseEvent& event)
{
    if (isMouseWithinDisc(event.position))
    {
        startPoint = event.position.toFloat();
        startAngle = angleFromCenter(startPoint);
    }
}

//track drag distance from startpoint
void TurntableComponent::mouseDrag(const juce::MouseEvent& event)
{
    if (isMouseWithinDisc(event.position))
    {
        static float lastAngle = 0; //keep track of the last angle

        float currentAngle = angleFromCenter(event.position);
        float angleDifference = currentAngle - startAngle;

        //adjust jumps in angles from quadrant to quadrant
        if (fabs(angleDifference - lastAngle) > M_PI) { //if absolute difference from current and previous angles > 180 degrees
            if (angleDifference > 0) {
                angleDifference -= 2 * M_PI; //adjust angleDifference by -360 degrees
            }
            else {
                angleDifference += 2 * M_PI; //adjust angleDifference by 360 degrees
            }
        }
        lastAngle = angleDifference; //store last angle for next call

        //update the disc rotation angle using rotationUpdate function
        rotationUpdate(radiansToDegrees(angleDifference)); //convert to degrees 

        //startAngle is updated to handle continuous dragging
        startAngle = currentAngle;

        //broadcast angleDifference to the registered listeners
        broadcastAngleChange(angleDifference);
    }
}

//to add or remove registered listeners
void TurntableComponent::addAngleChangeListener(AngleChangeListener* listener)
{
    angleChangeListeners.add(listener);
}

void TurntableComponent::removeAngleChangeListener(AngleChangeListener* listener)
{
    angleChangeListeners.removeAllInstancesOf(listener);
}

void TurntableComponent::broadcastAngleChange(float angleDifference)
{
    for (auto* listener : angleChangeListeners)
    {
        listener->angleChanged(angleDifference);
    }
}