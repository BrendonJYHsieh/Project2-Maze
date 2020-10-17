#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED
#include<cmath>

using namespace std;

namespace gm
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief A single 2D point on the coordinate system
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct Point
    {
        double x;
        double y;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Display the point's coordinates in the console
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        void printCoord()
        {
            cout<<x<<" "<<y<<endl;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Default constructor. Sets the coordinates of the point to (0;0)
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        Point()
        {
            x = 0;
            y = 0;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief The constructor requires the X and Y coordinate of the gm::Point
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        Point(double xCoordinate, double yCoordinate)
        {
            x = xCoordinate;
            y = yCoordinate;
        }
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief A straight line that goes through 2 gm::Points or is given parametres according to the
    /// equation: ax + b = y
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct Line
    {
        double a;
        double b;
        bool up=false;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief A function that requires 2 gm::Points to either return "a" and "b" or set "up" to true,
        /// which means that the gm::Line is parallel to the Y axis. In this case the X coordinate is a
        /// constant and both "a" and "b" will be set to X.
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        void findAB (Point A, Point B)
        {
            if (A.x == B.x)
            {
                a=A.x;
                b=A.x;
                up=true;
            }
            else if (A.y == B.y)
            {
                a=0;
                b=A.y;
                up=false;
            }
            else
            {
                a=(A.y-B.y)/(A.x-B.x);
                b=A.y-A.x*a;
                up=false;
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief A function that requires a given Y coordinate to return the X coordinate of a point with
        /// coordinates (X,Y) that lies on the line. If the line is parallel to the Y axis it returns the same
        /// given input.
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        double findY (double x)
        {
            if (up == false)
                return a*x+b;
            return x;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief A function that requires a given X coordinate to return the Y coordinate of a point with
        /// coordinates (X,Y) that lies on the line. If the line is parallel to the X axis it returns the same
        /// given input.
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        double findX (double y)
        {
            return (y-b)/a;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief A function that prints the parametres "a" and "b" that correspond to the equation of the
        /// current line
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        void printAB()
        {
            if (up == false) cout<<a<<"*x + "<<b<<endl;
            else cout<<"The line is parallel to the Y axis with constant X coordinate : "<<a<<endl;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief A function that returns a _random_ gm::Point that lies on the line with an X coordinate
        /// between 1 and 5000
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        Point generateRandomPoint()
        {
            double x = rand()%5000+1;
            Point ret;
            if (up == false)
            {
                double y = findY(x);
                ret.x = x;
                ret.y = y;
            }
            else
            {
                ret.y = x;
                ret.x = a;
            }
            return ret;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Default constructor. The line follows the equation: 0x + 0 = y (In other words is the X axis)
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        Line()
        {
            a = 0;
            b = 0;
            up = false;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief The constructor requires to be given the parametres of the equation: ax + b = y,
        /// where x and y are the coordinates of the points that create the line
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        Line(double aParametre, double bParametre)
        {
            a = aParametre;
            b = bParametre;
            up = false;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief The constructor requires to be given 2 different gm::Points that the Line goes through
        /// where x and y are the coordinates of the points that create the line
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        Line(point pointA, point pointB)
        {
            a = aParametre;
            b = bParametre;
            up = false;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief The constructor requires the X coordinate of a line that is parallel to the Y axis
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        Line(double xConst)
        {
            a = xConst;
            b = xConst;
            up = true;
        }
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    Line makeLine (Point x, Point y)
    {
        Line ret;
        ret.findAB(x, y);
        return ret;
    }

    struct Segment
    {
        Point A;
        Point B;
        double getLength()
        {
            return sqrt(abs(A.x-B.x)*abs(A.x-B.x) + abs(A.y-B.y)*abs(A.y-B.y));
        }
        Point findMid()
        {
            Point ret;
            ret.x = (A.x + B.x) / 2;
            ret.y = (A.y + B.y) / 2;
            return ret;
        }
        Point generateRandomPoint()
        {
            Point ret;
            if (A.x == B.x)
            {
                ret.x = A.x;
                ret.y = rand()%int(abs(A.y-B.y))+min(A.y,B.y);
            }
            else if (A.y == B.x)
            {
                ret.y = A.y;
                ret.x = rand()%int(abs(A.x-B.x))+min(A.x,B.x);
            }
            else
            {
                Line r = makeLine(A,B);
                ret.x = rand()%int(abs(A.x-B.x))+min(A.x,B.x);
                ret.y = r.findY(ret.x);
            }
            return ret;
        }
    };

    struct Circle
    {
        double radius;
        Point center;
    };

    Point makePoint (double x, double y)
    {
        Point ret;
        ret.x = x;
        ret.y = y;
        return ret;
    }

    Segment makeSegment (Point x, Point y)
    {
        Segment ret;
        ret.A = x;
        ret.B = y;
        return ret;
    }

    Line makeLine (Segment l)
    {
        Line ret;
        Point x = l.A, y = l.B;
        ret.findAB(x, y);
        return ret;
    }

    bool doesPointLay (Point p , Segment l)
    {
        Line r;
        r.findAB(l.A , l.B);

        if (r.up == true && l.A.x == p.x && min(l.A.y,l.B.y) <= p.y && max(l.A.y,l.B.y) >= p.y ) return true;
        else if (r.up == true) return false;

        if (p.y != r.findY(p.x)) return false;
        if (p.x >= min(l.A.x , l.B.x) && p.x <= max(l.A.x , l.B.x)) return true;
        return false;
    }

    Point findCrossPoint(Line x, Line y)
    {
        Point ret;
        if (x.up == true)
        {
            ret.y = y.findY(x.a);
            ret.x = x.a;
        }
        else if (y.up == true)
        {
            ret.y = x.findY(y.a);
            ret.x = y.a;
        }
        else
        {
            ret.x = (y.b-x.b)/(x.a-y.a);
            ret.y = x.a * ret.x + x.b;
        }
        return ret;
    }

    bool isParallel (Segment l1, Segment l2)
    {
        Line r1,r2;
        r1.findAB(l1.A, l1.B);
        r2.findAB(l2.A, l2.B);
        if (r1.a == r2.a) return true;
        else return false;
    }

    bool doSegmentsIntercept (Segment l1, Segment l2)
    {
        Line r1,r2;
        r1.findAB(l1.A, l1.B);
        r2.findAB(l2.A, l2.B);

        if (r1.a == r2.a && r1.b == r2.b) return true;

        Point p=findCrossPoint(gm::makeLine(l1), gm::makeLine(l2));
        if (doesPointLay(p, l1) && doesPointLay(p, l2)) return true;
        else return false;
    }

    Segment findHeigth(Segment L, Point X)
    {
        Point h;
        Segment ret;
        ret.A = X;

        if (L.A.y == L.B.y)
        {
            ret.B.x = X.x;
            ret.B.y = L.A.y;
        }
        else
        {
            h.x = X.x - L.B.y + L.A.y ;
            h.y = X.y + L.B.x - L.A.x ;
            ret.B = h;
            ret.B = findCrossPoint( makeLine(h , X) , makeLine(L.A , L.B) );
        }


        return ret;
    }

    Segment findHeigth(Line R, Point X)
    {
        Segment L;
        L.A = R.generateRandomPoint();
        L.B = R.generateRandomPoint();
        if (R.up == true)
        {
            Segment ret;
            ret.A = X;
            ret.B = makePoint(L.A.x, X.y);
            return ret;
        }
        Point h;
        Segment ret;
        ret.A = X;

        if (L.A.y == L.B.y)
        {
            ret.B.x = X.x;
            ret.B.y = L.A.y;
        }
        else
        {
            h.x = X.x - L.B.y + L.A.y ;
            h.y = X.y + L.B.x - L.A.x ;
            ret.B = h;
            ret.B = findCrossPoint( makeLine(h , X) , makeLine(L.A , L.B) );
        }


        return ret;
    }

    struct triangle
    {
        Point A;
        Point B;
        Point C;

        Segment medA;
        Segment medB;
        Segment medC;

        Segment bisecA;
        Segment bisecB;
        Segment bisecC;

        Segment heA;
        Segment heB;
        Segment heC;

        Segment simetralaAB;
        Segment simetralaBC;
        Segment simetralaAC;

        Circle excircle;
        Circle incircle;
        double Size;

        void simetrali()
        {
            Point a1,b1,diff;

            diff.x = B.x-A.x;
            diff.y = B.y-A.y;
            a1.x = B.x-(diff.x/2);
            a1.y = B.y-(diff.y/2);
            b1.x = B.x-diff.y-(diff.x/2);
            b1.y = B.y+diff.x-(diff.y/2);
            simetralaAB.A = a1;
            simetralaAB.B = b1;

            diff.x = C.x-B.x;
            diff.y = C.y-B.y;
            a1.x = C.x-(diff.x/2);
            a1.y = C.y-(diff.y/2);
            b1.x = C.x-diff.y-(diff.x/2);
            b1.y = C.y+diff.x-(diff.y/2);
            simetralaBC.A = a1;
            simetralaBC.B = b1;

            diff.x = A.x-C.x;
            diff.y = A.y-C.y;
            a1.x = A.x-(diff.x/2);
            a1.y = A.y-(diff.y/2);
            b1.x = A.x-diff.y-(diff.x/2);
            b1.y = A.y+diff.x-(diff.y/2);
            simetralaAC.A = a1;
            simetralaAC.B = b1;
        }

        void findSize()
        {
            double a,b,c,X;
            a = sqrt((A.x-B.x)*(A.x-B.x) + (A.y-B.y)*(A.y-B.y));
            b = sqrt((B.x-C.x)*(C.x-B.x) + (C.y-B.y)*(C.y-B.y));
            c = sqrt((A.x-C.x)*(A.x-C.x) + (A.y-C.y)*(A.y-C.y));
            X = (a+b+c)/2;
            Size = sqrt(X*(X-a)*(X-b)*(X-c));
        }

        void findMedians()
        {
            Point sec;
            medA.A=A;
            sec.x=(B.x+C.x)/2;
            sec.y=(B.y+C.y)/2;
            medA.B=sec;

            medB.A=B;
            sec.x=(A.x+C.x)/2;
            sec.y=(A.y+C.y)/2;
            medB.B=sec;

            medC.A=C;
            sec.x=(B.x+A.x)/2;
            sec.y=(B.y+A.y)/2;
            medC.B=sec;
        }
        void findBisectors()
        {
            double AB,BC,AC;
            double x,y;
            double a,b,a1,b1;
            double k;
            Line curr;

            AB = sqrt(abs(A.x-B.x)*abs(A.x-B.x) + abs(A.y-B.y)*abs(A.y-B.y));
            BC = sqrt(abs(C.x-B.x)*abs(C.x-B.x) + abs(C.y-B.y)*abs(C.y-B.y));
            AC = sqrt(abs(A.x-C.x)*abs(A.x-C.x) + abs(A.y-C.y)*abs(A.y-C.y));


            a=abs(B.x-C.x);
            b=abs(B.y-C.y);

            k=BC*AB / (AB+AC);
            b1=abs(sqrt((k*k * b*b) / (a*a + b*b)));
            a1=abs(a*b1/b);

            bisecA.A = A;
            if (B.y < C.y)
            {
                bisecA.B.y = B.y+b1;
            }
            if (B.x < C.x)
            {
                bisecA.B.x = B.x+a1;
            }
            if (B.y > C.y)
            {
                bisecA.B.y = B.y-b1;
            }
            if (B.x > C.x)
            {
                bisecA.B.x = B.x-a1;
            }
            if (B.x == C.x)
            {
                bisecA.B.x = B.x;
            }



            a=abs(C.x-A.x);
            b=abs(C.y-A.y);

            k=AC*BC / (BC+AB);
            b1=abs(sqrt((k*k * b*b) / (a*a + b*b)));
            a1=abs(a*b1/b);

            bisecB.A = B;
            if (C.y < A.y)
            {
                bisecB.B.y = C.y+b1;
            }
            if (C.x < A.x)
            {
                bisecB.B.x = C.x+a1;
            }
            if (C.y > A.y)
            {
                bisecB.B.y = C.y-b1;
            }
            if (C.x > A.x)
            {
                bisecB.B.x = C.x-a1;
            }
            if (C.x == A.x)
            {
                bisecB.B.x = C.x;
            }
            if (C.x == A.x)
            {
                bisecB.B.x = C.x;
            }


            a=abs(A.x-B.x);
            b=abs(A.y-B.y);

            k=AB*AC / (AC+BC);
            b1=abs(sqrt((k*k * b*b) / (a*a + b*b)));
            a1=abs(a*b1/b);

            bisecC.A = C;
            if (A.y < B.y)
            {
                bisecC.B.y = A.y+b1;
            }
            if (A.x < B.x)
            {
                bisecC.B.x = A.x+a1;
            }
            if (A.y > B.y)
            {
                bisecC.B.y = A.y-b1;
            }
            if (A.x > B.x)
            {
                bisecC.B.x = A.x-a1;
            }
            if (A.x == B.x)
            {
                bisecC.B.x = A.x;
            }
            if (A.x == B.x)
            {
                bisecC.B.x = A.x;
            }


            if (B.y == C.y)
            {
                Point r1 = findCrossPoint( makeLine(bisecB.A,bisecB.B) , makeLine(bisecC.A,bisecC.B));
                Point r2 = findCrossPoint( makeLine(B,C) , makeLine(r1,A));
                bisecA.B = r2;
            }

            if (C.y == A.y)
            {
                Point r1 = findCrossPoint( makeLine(bisecC.A,bisecC.B) , makeLine(bisecA.A,bisecA.B));
                Point r2 = findCrossPoint( makeLine(C,A) , makeLine(r1,B));
                bisecB.B = r2;
            }
            if (A.y == B.y)
            {
                Point r1 = findCrossPoint( makeLine(bisecA.A,bisecA.B) , makeLine(bisecB.A,bisecB.B));
                Point r2 = findCrossPoint( makeLine(A,B) , makeLine(r1,C));
                bisecC.B = r2;
            }

        }
        void findHeigths()
        {
            Segment r;

            r = findHeigth(makeSegment(A,B) , C);
            heA.A = r.A;
            heA.B = r.B;

            r = findHeigth(makeSegment(B,C) , A);
            heB.A = r.A;
            heB.B = r.B;

            r = findHeigth(makeSegment(C,A) , B);
            heC.A = r.A;
            heC.B = r.B;
        }

        void findExcircle()
        {
            simetrali();
            excircle.center = findCrossPoint(makeLine(simetralaBC.A , simetralaBC.B) , makeLine(simetralaAC.A , simetralaAC.B));
            excircle.radius = sqrt((excircle.center.x-A.x)*(excircle.center.x-A.x) + (excircle.center.y-A.y)*(excircle.center.y-A.y));
        }

        void findIncircle()
        {
            findBisectors();
            incircle.center = findCrossPoint(makeLine(bisecA.A , bisecA.B) , makeLine(bisecB.A , bisecB.B));
            Segment rad = findHeigth(makeSegment(A, B) , incircle.center);
            incircle.radius = sqrt(abs(rad.A.x - rad.B.x)*abs(rad.A.x - rad.B.x) + abs(rad.A.y - rad.B.y)*abs(rad.A.y - rad.B.y));
        }

        void findAll()
        {
            findMedians();
            findBisectors();
            findHeigths();
            simetrali();
            findSize();
            findExcircle();
            findIncircle();
        }
    };

    vector<Point> findCrossPoint(Circle c , Segment l)
    {
        vector<Point> ret;

        Segment h = findHeigth(l , c.center);

        Line r = makeLine(l);
        double a,b,A,B,R;
        a = r.a;
        b = r.b;
        A = c.center.x;
        B = c.center.y;
        R = c.radius;
        double D = R*R + a*a*R*R - pow(a*A - B + b , 2);

        if (h.getLength() > c.radius) ;
        else if (h.getLength() == c.radius)
            ret.push_back(h.B);
        else
        {
            if (r.up == true)
            {
                Point p1,p2;
                p1.x = r.a;
                p1.y = B + sqrt( R*R - (A-a)*(A-a) );
                p2.x = r.b;
                p2.y = B - sqrt( R*R - (A-a)*(A-a) );
                ret.push_back(p1);
                ret.push_back(p2);
            }
            else
            {
                Point p1,p2;
                p1.x = (A + B*a - a*b + sqrt(D))/(a*a + 1);
                p2.x = (A + B*a - a*b - sqrt(D))/(a*a + 1);
                p1.y = r.findY(p1.x);
                p2.y = r.findY(p2.x);
                ret.push_back(p1);
                ret.push_back(p2);
            }

        }

        return ret;
    }

    Circle makeCircle (Point c , double r)
    {
        Circle ret;
        ret.center = c;
        ret.radius = r;
        return ret;
    }

    vector<Point> findCrossPoint(Circle c1 , Circle c2)
    {
        vector<Point> ret;
        double b = c1.radius;
        double a = c2.radius;
        double c = makeSegment(c1.center, c2.center).getLength();
        double hp = (a+b+c)/2;
        double s = sqrt(hp*(hp-a)*(hp-b)*(hp-c));
        double heigth =s*2/c;

        double x = (b*b + c*c - a*a) / (2*c);
        vector<Point> v = findCrossPoint ( makeCircle(c1.center , x) , makeSegment(c1.center , c2.center));

        c1.center.printCoord();
        c2.center.printCoord();

        v[0].printCoord();
        v[1].printCoord();

        ret.push_back(v[0]);
        ret.push_back(v[1]);

        return ret;
    }

}

#endif // GEOMETRY_H_INCLUDED

