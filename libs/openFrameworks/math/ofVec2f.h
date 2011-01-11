#pragma once

#include "ofConstants.h"
#include "ofTypes.h"



class ofVec2f : public ofPoint {


  public:

    ofVec2f( float _x=0.0f, float _y=0.0f );
    ofVec2f( const ofPoint& pnt );


    // Getters and Setters.
    //
    void set( float _x, float _y );
    void set( const ofPoint& vec );
    float &operator[]( const int& i );


    // Check similarity/equality.
    //
    bool operator==( const ofPoint& vec );
    bool operator!=( const ofPoint& vec );
    bool match( const ofPoint& vec, float tollerance=0.0001 );
    /**
    * Checks if vectors look in the same direction.
    * Tollerance is specified in degree.
    */
    bool align( const ofVec2f& vec, float tollerance=0.0001 ) const;
    bool alignRad( const ofVec2f& vec, float tollerance=0.0001 ) const;


    // Overloading for any type to any type
    //
    void 	  operator=( const ofPoint& vec );
    ofVec2f  operator+( const ofPoint& vec ) const;
    ofVec2f& operator+=( const ofPoint& vec );
    ofVec2f  operator-( const ofPoint& vec ) const;
    ofVec2f& operator-=( const ofPoint& vec );
    ofVec2f  operator*( const ofPoint& vec ) const;
    ofVec2f& operator*=( const ofPoint& vec );
    ofVec2f  operator/( const ofPoint& vec ) const;
    ofVec2f& operator/=( const ofPoint& vec );


    //operator overloading for float
    //
    void 	  operator=( const float f);
    ofVec2f  operator+( const float f ) const;
    ofVec2f& operator+=( const float f );
    ofVec2f  operator-( const float f ) const;
    ofVec2f& operator-=( const float f );
    ofVec2f  operator-() const;
    ofVec2f  operator*( const float f ) const;
    ofVec2f& operator*=( const float f );
    ofVec2f  operator/( const float f ) const;
    ofVec2f& operator/=( const float f );


    // Scaling
    //
    ofVec2f  getScaled( const float length ) const;
    ofVec2f& scale( const float length );


    // Rotation
    //
    ofVec2f  getRotated( float angle ) const;
    ofVec2f  getRotatedRad( float angle ) const;
    ofVec2f& rotate( float angle );
    ofVec2f& rotateRad( float angle );

    
    // Rotation - point around pivot
    //
    ofVec2f  getRotated( float angle, const ofPoint& pivot ) const;
    ofVec2f& rotate( float angle, const ofPoint& pivot );
    ofVec2f  getRotatedRad( float angle, const ofPoint& pivot ) const;
    ofVec2f& rotateRad( float angle, const ofPoint& pivot );
        

    // Map point to coordinate system defined by origin, vx, and vy.
    //
    ofVec2f getMapped( const ofPoint& origin,
                        const ofVec2f& vx,
                        const ofVec2f& vy ) const;
    ofVec2f& map( const ofPoint& origin,
                   const ofVec2f& vx, const ofVec2f& vy );


    // Distance between two points.
    //
    float distance( const ofPoint& pnt) const;
    float squareDistance( const ofPoint& pnt ) const;


    // Linear interpolation.
    //
    //
    /**
    * p==0.0 results in this point, p==0.5 results in the
    * midpoint, and p==1.0 results in pnt being returned.
    */
    ofVec2f   getInterpolated( const ofPoint& pnt, float p ) const;
    ofVec2f&  interpolate( const ofPoint& pnt, float p );
    ofVec2f   getMiddle( const ofPoint& pnt ) const;
    ofVec2f&  middle( const ofPoint& pnt );
    ofVec2f&  average( const ofPoint* points, int num );
    
    
    // Normalization
    //
    ofVec2f  getNormalized() const;
    ofVec2f& normalize();


    // Limit length.
    //
	ofVec2f  getLimited(float max) const;
    ofVec2f& limit(float max);


    // Perpendicular normalized vector.
    //
    ofVec2f  getPerpendicular() const;
    ofVec2f& perpendicular();


    // Length
    //
    float length() const;
    float squareLength() const;


    /**
    * Angle (deg) between two vectors.
    * This is a signed relative angle between -180 and 180.
    */
    float angle( const ofVec2f& vec ) const;
    float angleRad( const ofVec2f& vec ) const;


    /**
    * Dot Product.
    */
    float dot( const ofVec2f& vec ) const;



    //---------------------------------------------------
    // this methods are deprecated in 006 please use:

    // getScaled
    ofVec2f rescaled( const float length ) const;

    // scale
    ofVec2f& rescale( const float length );

    // getRotated
    ofVec2f rotated( float angle ) const;

    // getNormalized
    ofVec2f normalized() const;

    // getLimited
    ofVec2f limited(float max) const;

    // getPerpendicular
    ofVec2f perpendiculared() const;

    // squareLength
    float lengthSquared() const;

    // getInterpolated
    ofVec2f interpolated( const ofPoint& pnt, float p ) const;
    
    // getMiddled
    ofVec2f middled( const ofPoint& pnt ) const;
    
    // getMapped 
    ofVec2f mapped( const ofPoint& origin, const ofVec2f& vx, const ofVec2f& vy ) const;
    
    // squareDistance
    float distanceSquared( const ofPoint& pnt ) const;
    
    // use getRotated
    ofVec2f rotated( float angle, const ofPoint& pivot ) const;    
};





// Non-Member operators
//
ofVec2f operator+( float f, const ofVec2f& vec );
ofVec2f operator-( float f, const ofVec2f& vec );
ofVec2f operator*( float f, const ofVec2f& vec );
ofVec2f operator/( float f, const ofVec2f& vec );







/////////////////
// Implementation
/////////////////


inline ofVec2f::ofVec2f( float _x, float _y ) {
	x = _x;
	y = _y;
}

inline ofVec2f::ofVec2f( const ofPoint& pnt ) {
	x = pnt.x;
	y = pnt.y;
}



// Getters and Setters.
//
//
inline void ofVec2f::set( float _x, float _y ) {
	x = _x;
	y = _y;
}

inline void ofVec2f::set( const ofPoint& vec ) {
	x = vec.x;
	y = vec.y;
}

inline float& ofVec2f::operator[]( const int& i ) {
	switch(i) {
		case 0:  return x;
		case 1:  return y;
		default: return x;
	}
}



// Check similarity/equality.
//
//
inline bool ofVec2f::operator==( const ofPoint& vec ) {
	return (x == vec.x) && (y == vec.y);
}

inline bool ofVec2f::operator!=( const ofPoint& vec ) {
	return (x != vec.x) || (y != vec.y);
}

inline bool ofVec2f::match( const ofPoint& vec, float tollerance ) {
	return (fabs(x - vec.x) < tollerance)
		&& (fabs(y - vec.y) < tollerance);
}

/**
* Checks if vectors look in the same direction.
* Tollerance is specified in degree.
*/
inline bool ofVec2f::align( const ofVec2f& vec, float tollerance ) const {
	return  fabs( this->angle( vec ) ) < tollerance;
}

inline bool ofVec2f::alignRad( const ofVec2f& vec, float tollerance ) const {
	return  fabs( this->angleRad( vec ) ) < tollerance;
}


// Overloading for any type to any type
//
//
inline void ofVec2f::operator=( const ofPoint& vec ){
	x = vec.x;
	y = vec.y;
}

inline ofVec2f ofVec2f::operator+( const ofPoint& vec ) const {
	return ofVec2f( x+vec.x, y+vec.y);
}

inline ofVec2f& ofVec2f::operator+=( const ofPoint& vec ) {
	x += vec.x;
	y += vec.y;
	return *this;
}

inline ofVec2f ofVec2f::operator-( const ofPoint& vec ) const {
	return ofVec2f(x-vec.x, y-vec.y);
}

inline ofVec2f& ofVec2f::operator-=( const ofPoint& vec ) {
	x -= vec.x;
	y -= vec.y;
	return *this;
}

inline ofVec2f ofVec2f::operator*( const ofPoint& vec ) const {
	return ofVec2f(x*vec.x, y*vec.y);
}

inline ofVec2f& ofVec2f::operator*=( const ofPoint& vec ) {
	x*=vec.x;
	y*=vec.y;
	return *this;
}

inline ofVec2f ofVec2f::operator/( const ofPoint& vec ) const {
	return ofVec2f( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y);
}

inline ofVec2f& ofVec2f::operator/=( const ofPoint& vec ) {
	vec.x!=0 ? x/=vec.x : x;
	vec.y!=0 ? y/=vec.y : y;
	return *this;
}


//operator overloading for float
//
//
inline void ofVec2f::operator=( const float f){
	x = f;
	y = f;
}

inline ofVec2f ofVec2f::operator+( const float f ) const {
	return ofVec2f( x+f, y+f);
}

inline ofVec2f& ofVec2f::operator+=( const float f ) {
	x += f;
	y += f;
	return *this;
}

inline ofVec2f ofVec2f::operator-( const float f ) const {
	return ofVec2f( x-f, y-f);
}

inline ofVec2f& ofVec2f::operator-=( const float f ) {
	x -= f;
	y -= f;
	return *this;
}

inline ofVec2f ofVec2f::operator-() const {
	return ofVec2f(-x, -y);
}

inline ofVec2f ofVec2f::operator*( const float f ) const {
	return ofVec2f(x*f, y*f);
}

inline ofVec2f& ofVec2f::operator*=( const float f ) {
	x*=f;
	y*=f;
	return *this;
}

inline ofVec2f ofVec2f::operator/( const float f ) const {
	if(f == 0) return ofVec2f(x, y);

	return ofVec2f(x/f, y/f);
}

inline ofVec2f& ofVec2f::operator/=( const float f ) {
	if(f == 0) return *this;

	x/=f;
	y/=f;
	return *this;
}

inline ofVec2f ofVec2f::rescaled( const float length ) const {
	return getScaled(length);
}

inline ofVec2f ofVec2f::getScaled( const float length ) const {
	float l = (float)sqrt(x*x + y*y);
	if( l > 0 )
		return ofVec2f( (x/l)*length, (y/l)*length );
	else
		return ofVec2f();
}

inline ofVec2f& ofVec2f::rescale( const float length ){
	return scale(length);
}

inline ofVec2f& ofVec2f::scale( const float length ) {
	float l = (float)sqrt(x*x + y*y);
	if (l > 0) {
		x = (x/l)*length;
		y = (y/l)*length;
	}
	return *this;
}



// Rotation
//
//
inline ofVec2f ofVec2f::rotated( float angle ) const {
	return getRotated(angle);
}

inline ofVec2f ofVec2f::getRotated( float angle ) const {
	float a = (float)(angle*DEG_TO_RAD);
	return ofVec2f( x*cos(a) - y*sin(a),
					x*sin(a) + y*cos(a) );
}

inline ofVec2f ofVec2f::getRotatedRad( float angle ) const {
	float a = angle;
	return ofVec2f( x*cos(a) - y*sin(a),
					x*sin(a) + y*cos(a) );
}

inline ofVec2f& ofVec2f::rotate( float angle ) {
	float a = (float)(angle * DEG_TO_RAD);
	float xrot = x*cos(a) - y*sin(a);
	y = x*sin(a) + y*cos(a);
	x = xrot;
	return *this;
}

inline ofVec2f& ofVec2f::rotateRad( float angle ) {
	float a = angle;
	float xrot = x*cos(a) - y*sin(a);
	y = x*sin(a) + y*cos(a);
	x = xrot;
	return *this;
}



// Rotate point by angle (deg) around pivot point.
//
//

// This method is deprecated in 006 please use getRotated instead
inline ofVec2f ofVec2f::rotated( float angle, const ofPoint& pivot ) const {
	return getRotated(angle, pivot);
}

inline ofVec2f ofVec2f::getRotated( float angle, const ofPoint& pivot ) const {
	float a = (float)(angle * DEG_TO_RAD);
	return ofVec2f( ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x,
                     ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y );
}

inline ofVec2f& ofVec2f::rotate( float angle, const ofPoint& pivot ) {
	float a = (float)(angle * DEG_TO_RAD);
	float xrot = ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x;
	y = ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y;
	x = xrot;
	return *this;
}

inline ofVec2f ofVec2f::getRotatedRad( float angle, const ofPoint& pivot ) const {
	float a = angle;
	return ofVec2f( ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x,
                     ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y );
}

inline ofVec2f& ofVec2f::rotateRad( float angle, const ofPoint& pivot ) {
	float a = angle;
	float xrot = ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x;
	y = ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y;
	x = xrot;
	return *this;
}



// Map point to coordinate system defined by origin, vx, and vy.
//
//

// This method is deprecated in 006 please use getMapped instead
inline ofVec2f ofVec2f::mapped( const ofPoint& origin,
					              const ofVec2f& vx,
					              const ofVec2f& vy ) const{
	return getMapped(origin, vx, vy);
}

inline ofVec2f ofVec2f::getMapped( const ofPoint& origin,
				                     const ofVec2f& vx,
				                     const ofVec2f& vy ) const
{
	return ofVec2f( origin.x + x*vx.x + y*vy.x,
                     origin.y + x*vx.y + y*vy.y );
}

inline ofVec2f& ofVec2f::map( const ofPoint& origin,
				                const ofVec2f& vx, const ofVec2f& vy )
{
	float xmap = origin.x + x*vx.x + y*vy.x;
	y = origin.y + x*vx.y + y*vy.y;
	x = xmap;
	return *this;
}


// Distance between two points.
//
//
inline float ofVec2f::distance( const ofPoint& pnt) const {
	float vx = x-pnt.x;
	float vy = y-pnt.y;
	return (float)sqrt(vx*vx + vy*vy);
}

//this method is deprecated in 006 please use squareDistance
inline float ofVec2f::distanceSquared( const ofPoint& pnt ) const {
	return squareDistance(pnt);
}

inline float ofVec2f::squareDistance( const ofPoint& pnt ) const {
	float vx = x-pnt.x;
	float vy = y-pnt.y;
	return vx*vx + vy*vy;
}



// Linear interpolation.
//
//
/**
* p==0.0 results in this point, p==0.5 results in the
* midpoint, and p==1.0 results in pnt being returned.
*/

// this method is deprecated in 006 please use getInterpolated
inline ofVec2f ofVec2f::interpolated( const ofPoint& pnt, float p ) const{
	return getInterpolated(pnt, p);
}

inline ofVec2f ofVec2f::getInterpolated( const ofPoint& pnt, float p ) const {
	return ofVec2f( x*(1-p) + pnt.x*p, y*(1-p) + pnt.y*p );
}

inline ofVec2f& ofVec2f::interpolate( const ofPoint& pnt, float p ) {
	x = x*(1-p) + pnt.x*p;
	y = y*(1-p) + pnt.y*p;
	return *this;
}

// this method is deprecated in 006 please use getMiddle
inline ofVec2f ofVec2f::middled( const ofPoint& pnt ) const{
	return getMiddle(pnt);
}

inline ofVec2f ofVec2f::getMiddle( const ofPoint& pnt ) const {
	return ofVec2f( (x+pnt.x)/2.0f, (y+pnt.y)/2.0f );
}

inline ofVec2f& ofVec2f::middle( const ofPoint& pnt ) {
	x = (x+pnt.x)/2.0f;
	y = (y+pnt.y)/2.0f;
	return *this;
}



// Average (centroid) among points.
// Addition is sometimes useful for calculating averages too.
//
//
inline ofVec2f& ofVec2f::average( const ofPoint* points, int num ) {
	x = 0.f;
	y = 0.f;
	for( int i=0; i<num; i++) {
		x += points[i].x;
		y += points[i].y;
	}
	x /= num;
	y /= num;
	return *this;
}



// Normalization
//
//
inline ofVec2f ofVec2f::normalized() const {
	return getNormalized();
}

inline ofVec2f ofVec2f::getNormalized() const {
	float length = (float)sqrt(x*x + y*y);
	if( length > 0 ) {
		return ofVec2f( x/length, y/length );
	} else {
		return ofVec2f();
	}
}

inline ofVec2f& ofVec2f::normalize() {
	float length = (float)sqrt(x*x + y*y);
	if( length > 0 ) {
		x /= length;
		y /= length;
	}
	return *this;
}



// Limit length.
//
//
inline ofVec2f ofVec2f::limited(float max) const{
	return getLimited(max);
}

inline ofVec2f ofVec2f::getLimited(float max) const {
    ofVec2f limited;
    float lengthSquared = (x*x + y*y);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        float ratio = max/(float)sqrt(lengthSquared);
        limited.set( x*ratio, y*ratio);
    } else {
        limited.set(x,y);
    }
    return limited;
}

inline ofVec2f& ofVec2f::limit(float max) {
    float lengthSquared = (x*x + y*y);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        float ratio = max/(float)sqrt(lengthSquared);
        x *= ratio;
        y *= ratio;
    }
    return *this;
}



// Perpendicular normalized vector.
//
//
inline ofVec2f ofVec2f::perpendiculared() const {
	return getPerpendicular();
}

inline ofVec2f ofVec2f::getPerpendicular() const {
	float length = (float)sqrt( x*x + y*y );
	if( length > 0 )
		return ofVec2f( -(y/length), x/length );
	else
		return ofVec2f();
}

inline ofVec2f& ofVec2f::perpendicular() {
	float length = (float)sqrt( x*x + y*y );
	if( length > 0 ) {
		float _x = x;
		x = -(y/length);
		y = _x/length;
	}
	return *this;
}


// Length
//
//
inline float ofVec2f::length() const {
	return (float)sqrt( x*x + y*y );
}

inline float ofVec2f::lengthSquared() const {
	return squareLength();
}

inline float ofVec2f::squareLength() const {
	return (float)(x*x + y*y);
}



/**
* Angle (deg) between two vectors.
* This is a signed relative angle between -180 and 180.
*/
inline float ofVec2f::angle( const ofVec2f& vec ) const {
	return (float)(atan2( x*vec.y-y*vec.x, x*vec.x + y*vec.y )*RAD_TO_DEG);
}

/**
* Angle (deg) between two vectors.
* This is a signed relative angle between -180 and 180.
*/
inline float ofVec2f::angleRad( const ofVec2f& vec ) const {
	return atan2( x*vec.y-y*vec.x, x*vec.x + y*vec.y );
}


/**
* Dot Product.
*/
inline float ofVec2f::dot( const ofVec2f& vec ) const {
	return x*vec.x + y*vec.y;
}







// Non-Member operators
//
//
inline ofVec2f operator+( float f, const ofVec2f& vec ) {
    return ofVec2f( f+vec.x, f+vec.y);
}

inline ofVec2f operator-( float f, const ofVec2f& vec ) {
    return ofVec2f( f-vec.x, f-vec.y);
}

inline ofVec2f operator*( float f, const ofVec2f& vec ) {
    return ofVec2f( f*vec.x, f*vec.y);
}

inline ofVec2f operator/( float f, const ofVec2f& vec ) {
    return ofVec2f( f/vec.x, f/vec.y);
}


