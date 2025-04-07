#include <iostream>
#include <cmath>

class Vector3F {
private:
    float x, y, z;
    int State; // 0 - OK, 1 - nullptr, 2 - divide by zero, 3 - invalid index
    static int objectCount;

public:
    // Default constructor
    Vector3F() : x(0), y(0), z(0), State(0) {
        objectCount++;
    }

    // Constructor with one float
    Vector3F(float value) : x(value), y(value), z(value), State(0) {
        objectCount++;
    }

    // Constructor with pointer to array
    Vector3F(const float* arr) {
        if (arr == nullptr) {
            x = y = z = 0;
            State = 1;
        }
        else {
            x = arr[0]; y = arr[1]; z = arr[2];
            State = 0;
        }
        objectCount++;
    }

    // Destructor
    ~Vector3F() {
        std::cout << "Destroyed vector (" << x << ", " << y << ", " << z << ") State: " << State << "\n";
        objectCount--;
    }

    // Get object count
    static int getObjectCount() {
        return objectCount;
    }

    // Print vector
    void print() const {
        std::cout << "Vector(" << x << ", " << y << ", " << z << ") State: " << State << "\n";
    }

    // --- Unary operators ---
    Vector3F& operator++() { x += 1.0f; y += 1.0f; z += 1.0f; return *this; }
    Vector3F operator++(int) { Vector3F temp = *this; ++(*this); return temp; }
    Vector3F& operator--() { x -= 1.0f; y -= 1.0f; z -= 1.0f; return *this; }
    Vector3F operator--(int) { Vector3F temp = *this; --(*this); return temp; }
    bool operator!() const { return !(x || y || z); }
    Vector3F operator~() const { return Vector3F(-y, x, z); } // perpendicular (in 2D xy)
    Vector3F operator-() const { return Vector3F(-x, -y, -z); }

    // --- Assignment ---
    Vector3F& operator=(const Vector3F& other) {
        if (this != &other) {
            x = other.x; y = other.y; z = other.z;
            State = other.State;
        }
        return *this;
    }

    // --- Compound assignment ---
    Vector3F& operator+=(const Vector3F& other) { x += other.x; y += other.y; z += other.z; return *this; }
    Vector3F& operator-=(const Vector3F& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
    Vector3F& operator*=(float val) { x *= val; y *= val; z *= val; return *this; }
    Vector3F& operator/=(float val) {
        if (val == 0) { State = 2; return *this; }
        x /= val; y /= val; z /= val; return *this;
    }
    Vector3F& operator&=(const Vector3F& other) { // cross product
        float cx = y * other.z - z * other.y;
        float cy = z * other.x - x * other.z;
        float cz = x * other.y - y * other.x;
        x = cx; y = cy; z = cz;
        return *this;
    }
    Vector3F& operator%=(int val) {
        if (val == 0) { State = 2; return *this; }
        x = int(x) % val;
        y = int(y) % val;
        z = int(z) % val;
        return *this;
    }

    // --- Binary arithmetic ---
    Vector3F operator+(const Vector3F& other) const { return Vector3F(x + other.x, y + other.y, z + other.z); }
    Vector3F operator-(const Vector3F& other) const { return Vector3F(x - other.x, y - other.y, z - other.z); }
    Vector3F operator*(float val) const { return Vector3F(x * val, y * val, z * val); }
    Vector3F operator/(float val) const {
        if (val == 0) { Vector3F res = *this; res.State = 2; return res; }
        return Vector3F(x / val, y / val, z / val);
    }
    Vector3F operator%(int val) const {
        if (val == 0) { Vector3F res = *this; res.State = 2; return res; }
        return Vector3F(int(x) % val, int(y) % val, int(z) % val);
    }

    // --- Comparison ---
    bool operator==(const Vector3F& other) const { return x == other.x && y == other.y && z == other.z; }
    bool operator!=(const Vector3F& other) const { return !(*this == other); }
    bool operator>(const Vector3F& other) const { return x > other.x && y > other.y && z > other.z; }
    bool operator>=(const Vector3F& other) const { return x >= other.x && y >= other.y && z >= other.z; }
    bool operator<(const Vector3F& other) const { return x < other.x && y < other.y && z < other.z; }
    bool operator<=(const Vector3F& other) const { return x <= other.x && y <= other.y && z <= other.z; }

    // --- Index operator ---
    float& operator[](int index) {
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        State = 3;
        return z; // default return
    }

    // --- Function call operator ---
    void operator()() const {
        std::cout << "Called operator() - Vector3F: (" << x << ", " << y << ", " << z << ")\n";
    }

    // --- Friend operators for stream ---
    friend std::ostream& operator<<(std::ostream& out, const Vector3F& v);
    friend std::istream& operator>>(std::istream& in, Vector3F& v);

    // Additional constructor for internal use
    Vector3F(float x_, float y_, float z_) : x(x_), y(y_), z(z_), State(0) {
        objectCount++;
    }
};

int Vector3F::objectCount = 0;

std::ostream& operator<<(std::ostream& out, const Vector3F& v) {
    out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return out;
}

std::istream& operator>>(std::istream& in, Vector3F& v) {
    in >> v.x >> v.y >> v.z;
    return in;
}

// === MAIN ===
int main() {
    Vector3F a(1.0f), b(2.0f), c;
    c = a + b;
    c.print();

    c += b;
    c.print();

    c *= 2;
    c.print();

    Vector3F d = ~c;
    d.print();

    Vector3F e(nullptr); // error state
    e.print();

    c();
    std::cout << "c[1] = " << c[1] << "\n";

    std::cout << "Total objects: " << Vector3F::getObjectCount() << "\n";

    return 0;
}
