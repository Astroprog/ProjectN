//
//  Orbit.cpp
//  ProjectN
//
//  Created by Peter Rodenkirch on 02.09.15.
//
//

#include "Orbit.h"

using namespace cocos2d;


Orbit::Orbit()
{
    radius = 0.0f;
    radiusVec = Vec2::ZERO;
    velocity = 0.0f;
    velocityVec = Vec2::ZERO;
    centralMass = 0.0f;
    energy = 0.0f;
    semiMajorAxis = 0.0f;
    semiMinorAxis = 0.0f;
    eccentricity = 0.0f;
    eccentricityVec = Vec2::ZERO;
    trueAnomaly = 0.0f;
    eccentricAnomaly = 0.0f;
    meanAnomaly = 0.0f;
    periapsisDistance = 0.0f;
    apoapsisDistance = 0.0f;
    center = Vec2::ZERO;
    inclinaton = 0.0f;
    semiLatusRectum = 0.0f;
    orbitPeriod = 0.0f;
}

void Orbit::computeOrbitalParameters()
{
    radius = radiusVec.length();
    velocity = velocityVec.length();
    
    //Energy
    
    energy = pow(velocity, 2) / 2 - centralMass / radius;
    printf("\n\nEnergy: %f\n", energy);
    
    //Major Axes
    
    semiMajorAxis = - centralMass / (2 * energy);
//    printf("Semi Major Axis: %f\n", semiMajorAxis);
    semiMinorAxis = semiMajorAxis * sqrt(1 - pow(eccentricity, 2));
//    printf("Semi Minor Axis: %f\n", semiMinorAxis);
    
    //Eccentricity

    eccentricityVec = pow(velocity, 2) * radiusVec / centralMass - radiusVec.dot(velocityVec) * velocityVec / centralMass - radiusVec / radius;
//    printf("EccentricityVec x: %f, y: %f\n", eccentricityVec.x, eccentricityVec.y);
    //    eccentricity = sqrt(1 - pow(radiusVec.cross(velocityVec), 2) / (semiMajorAxis * centralMass));
    eccentricity = eccentricityVec.length();
    printf("Eccentricity: %f\n", eccentricity);
    
    
    //True Anomaly
    
    
    if (radiusVec.dot(velocityVec) >= 0) {
        trueAnomaly = acos(eccentricityVec.dot(radiusVec)/(eccentricity * radius));
    } else {
        trueAnomaly = 2 * M_PI - acos(eccentricityVec.dot(radiusVec)/(eccentricity * radius));
    }
    
//    printf("True Anomaly: %f\n", trueAnomaly);
    
    //Eccentric Anomaly
    
    if (radiusVec.dot(velocityVec) >= 0) {
        eccentricAnomaly = acos((eccentricity + cos(trueAnomaly)) / (1 + eccentricity * cos(trueAnomaly)));
    } else {
         eccentricAnomaly = 2 * M_PI - acos((eccentricity + cos(trueAnomaly)) / (1 + eccentricity * cos(trueAnomaly)));
    }
    
    
    printf("Eccentric Anomaly: %f\n", eccentricAnomaly);
    
    //Mean Anomaly
    
    meanAnomaly = eccentricAnomaly - eccentricity * sin(eccentricAnomaly);
    
    printf("Mean Anomaly: %f\n", meanAnomaly);
    
    
    //Periapis, Apoapsis
    
    periapsisDistance = pow(radiusVec.cross(velocityVec), 2) / (centralMass * (1 + eccentricity));
    printf("Periapss Distance: %f\n", periapsisDistance);
    apoapsisDistance = pow(radiusVec.cross(velocityVec), 2) / (centralMass * (1 - eccentricity));
    printf("Apoapsis Distance: %f\n", apoapsisDistance);
    
    //Center of ellipse
    
    Vec2 eVec = eccentricityVec;
    eVec.negate();
    eVec.normalize();
    center = (semiMajorAxis - periapsisDistance) * eVec;
    printf("Center: %f, %f\n", center.x, center.y);
    
    //Inclination of ellipse (2D)
    
    inclinaton = angleVec(eccentricityVec, Vec2(0, -1));
    printf("Inclination: %f\n", inclinaton);
    printf("EccentricityVec x: %f, y: %f\n", eccentricityVec.x, eccentricityVec.y);
    
    //Semi-latus Rectum
    
    semiLatusRectum = semiMajorAxis * (1 - pow(eccentricity, 2));
//    printf("Semi-Latus Rectum: %f\n", semiLatusRectum);
    
    
    orbitPeriod = 2 * M_PI * sqrt(pow(semiMajorAxis, 3) / centralMass);
    printf("Orbit Period: %f\n", orbitPeriod);
    
    timeOfFlight = sqrtf(powf(semiMajorAxis, 3) / centralMass) * (eccentricAnomaly - eccentricity * sinf(eccentricAnomaly));
    printf("Time of Flight: %f\n", timeOfFlight);
    
    float E = eccentricAnomalyWithTime(timeOfFlight);
    printf("EccAnomCalc: %f", E);
}

void Orbit::updatePosition(float delta)
{
    timeOfFlight += delta;
    eccentricAnomaly = eccentricAnomalyWithTime(timeOfFlight);
    radius = semiMajorAxis * (1 - eccentricity * cosf(eccentricAnomaly));
}

float Orbit::meanAnomalyWithTime(float t)
{
    return t * sqrtf(centralMass / powf(semiMajorAxis, 3));
}

float Orbit::eccentricAnomalyWithTime(float t)
{
    float M = meanAnomalyWithTime(t);
    return eccentricAnomalyWithMeanAnomaly(M);
}

float Orbit::eccentricAnomalyWithMeanAnomaly(float M)
{
    float delta = 0.00001f;
    int maxI = 30;
    int i = 0;
    float E = 0.0f;
    
    if (eccentricity < 0.8) {
        E = M;
    } else {
        E = M_PI;
    }
    
    float F = E - eccentricity * sinf(M) - M;
    
    while ((fabsf(F) > delta) && (i < maxI)) {
        E = E - F / (1.0 - eccentricity * cosf(E));
        F = E - eccentricity * sinf(E) - M;
        i++;
    }
    
    return E;
}


inline float Orbit::angleVec(cocos2d::Vec2 vec1, cocos2d::Vec2 vec2)
{
    return atan2(vec1.cross(vec2), vec1.dot(vec2));
}




