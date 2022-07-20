#version 330 core
out vec4 FragColor;

uniform float time;
uniform vec2 resolution;
uniform vec3 position;
uniform vec2 viewDir;
uniform vec3 user;

uniform vec3 scene1Color;
uniform sampler2D texture01;
uniform sampler2D texture02;

#define PI 3.14159265359

float cameraFov = 0.75;

vec3 cameraPosition = vec3(0.0, 0.0, 5.0);
vec3 cameraLookAt = vec3(0.0, 0.0, 0.0);

vec3 omniLightPosition = vec3(0.0, 10.0, 10.0);

const float RAY_MAX_STEPS = 64.0;
const float RAY_THRESHOLD = 0.001;
const float RAY_MAX_DISTANCE = 200.0;

vec3 fogColor = vec3(0.5);
float fogIntensity = 0.000;

struct vec2Tuple {
    vec2 first;
    vec2 second;
};

struct vec3Tuple {
    vec3 first;
    vec3 second;
};

struct textureOptions {
    int index;
    vec3 baseColor;
    vec3 offset;
    vec3 scale;
};

struct ambientOptions {
    vec3 color;
    float strength;
};

struct diffuseOptions {
    vec3 color;
    float strength;
};

struct specularOptions {
    vec3 color;
    float strength;
    float shininess;
};

struct pointLight {
    vec3 position;
    vec3 color;
    float strength;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct shadowOptions {
    bool enabled;
    float lowerLimit;
    float upperLimit;
    float limit;
    float hardness;
};

struct aoOptions {
    bool enabled;
    float limit;
    float factor;
};

struct material {
    ambientOptions ambient;
    diffuseOptions diffuse;
    specularOptions specular;
    shadowOptions shadow;
    aoOptions ao;
    textureOptions texture;
};

struct entity {
    bool needNormals;
    vec4 trap;
    float dist;
    vec3 point;
    vec3 normal;
    material material;
};

struct hit {
    float steps;
    float dist;

    entity entity;
};

const int NUMBER_OF_POINT_LIGHTS = 2;
pointLight pointLights[NUMBER_OF_POINT_LIGHTS];

float opSmoothUnion(float d1, float d2, float k) {
    float h = clamp(0.5 + 0.5 * (d2 - d1) / k, 0.0, 1.0);
    return mix(d2, d1, h) - k * h * (1.0 - h);
}

float opSmoothUnion(float d1, float d2) {
    return opSmoothUnion(d1, d2, 0.0);
}

float opSmoothSubtraction(float d1, float d2, float k) {
    float h = clamp(0.5 - 0.5 * (d2 + d1) / k, 0.0, 1.0);
    return mix(d2, -d1, h) + k * h * (1.0 - h);
}

float opSmoothSubtraction(float d1, float d2) {
    return opSmoothSubtraction(d1, d2, 0.0);
}

float opSmoothIntersection(float d1, float d2, float k) {
    float h = clamp(0.5 - 0.5 * (d2 - d1) / k, 0.0, 1.0 );
    return mix(d2, d1, h) + k * h * (1.0 - h);
}

float opSmoothIntersection(float d1, float d2) {
    return opSmoothIntersection(d1, d2, 0.0);
}

entity opSmoothUnion(entity m1, entity m2, float k, float threshold) {
    float h = opSmoothUnion(m1.dist, m2.dist, k);
    if (smoothstep(m1.dist, m2.dist, h + threshold) > 0.5) {
        m2.dist = h;
        return m2;
    }
    else {
        m1.dist = h;
        return m1;
    }
}

entity opSmoothSubtraction(entity m1, entity m2, float k, float threshold) {
    float h = opSmoothSubtraction(m1.dist, m2.dist, k);
    if (smoothstep(m1.dist, m2.dist, h + threshold) > 0.5) {
        m2.dist = h;
        return m2;
    }
    else {
        m1.dist = h;
        return m1;
    }
}

entity opSmoothIntersection(entity m1, entity m2, float k, float threshold) {
    float h = opSmoothIntersection(m1.dist, m2.dist, k);
    if (smoothstep(m1.dist, m2.dist, h + threshold) > 0.5) {
        m2.dist = h;
        return m2;
    }
    else {
        m1.dist = h;
        return m1;
    }
}

vec3 opTwist(vec3 p, float angle) {
    float c = cos(angle * p.y);
    float s = sin(angle * p.y);
    mat2 m = mat2(c, -s, s, c);
    vec3 q = vec3(m * p.xz, p.y);
    return q;
}

vec3 opBend(vec3 p, float angle) {
    float c = cos(angle * p.y);
    float s = sin(angle * p.y);
    mat2 m = mat2(c, -s, s, c);
    vec3 q = vec3(m * p.xy, p.z);
    return q;
}

vec3 opElongate(vec3 p, vec3 h) {
    return p - clamp(p, -h, h);
}

float sdPlane(vec3 p, vec3 n, float h) {
    return dot(p, n) + h;
}

float sdSphere(vec3 p, float radius) {
    return length(p) - radius;
}

float sdEllipsoid(vec3 p, vec3 r) {
    float k0 = length(p / r);
    float k1 = length(p / (r * r));
    return k0 * (k0 - 1.0) / k1;
}

float sdBox(vec3 p, vec3 b, float r) {   
    vec3 d = abs(p) - b;
    return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0)) - r;
}

float sdTorus(vec3 p, vec2 t) {   
    vec2 q = vec2(length(p.xz) - t.x, p.y);
    return length(q) - t.y;
}

float sdCylinder(vec3 p, vec3 c, float r) {
    return length(p.xz - c.xy) - c.z - r;
}

float sdCappedCylinder(vec3 p, vec2 size, float r) {
    vec2 d = abs(vec2(length(p.xz), p.y)) - size;
    return min(max(d.x ,d.y), 0.0) + length(max(d, 0.0)) - r;
}

float sdRoundCone(in vec3 p, in float r1, float r2, float h) {    
    vec2 q = vec2(length(p.xz), p.y);
    
    float b = (r1 - r2) / h;
    float a = sqrt(1.0 - b * b);
    float k = dot(q, vec2(-b, a));
    
    if(k < 0.0) return length(q) - r1;
    if(k > a * h) return length(q - vec2(0.0, h)) - r2;
        
    return dot(q, vec2(a,b)) - r1;
}

float sdCapsule(vec3 p, vec3 a, vec3 b, float r) {   
    vec3 pa = p - a, ba = b - a;
    float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    return length(pa - ba * h) - r;
}

float sdHexagon(vec3 p, vec2 h, float r) {
    vec3 q = abs(p);
    return max(q.z - h.y, max((q.x * 0.866025 + q.y * 0.5), q.y) - h.x) - r;
}

float sdPyramid(vec3 p, float h) {
    float m2 = h * h + 0.25;

    p.xz = abs(p.xz);
    p.xz = (p.z > p.x) ? p.zx : p.xz;
    p.xz -= 0.5;

    vec3 q = vec3(p.z, h * p.y - 0.5 * p.x, h * p.x + 0.5 * p.y);

    float s = max(-q.x, 0.0);
    float t = clamp((q.y - 0.5 * p.z) / (m2 + 0.25), 0.0, 1.0);

    float a = m2 * (q.x + s) * (q.x + s) + q.y * q.y;
    float b = m2 * (q.x + 0.5 * t) * (q.x + 0.5 * t) + (q.y - m2 * t) * (q.y - m2 * t);

    float d2 = min(q.y, -q.x * m2 - q.y * 0.5) > 0.0 ? 0.0 : min(a,b);

    return sqrt((d2 + q.z * q.z) / m2) * sign(max(q.z, -p.y));
}

float sdOctahedron(vec3 p, float s) {
    p = abs(p);
    float m = p.x + p.y + p.z - s;
    vec3 q;
    if(3.0 * p.x < m ) q = p.xyz;
    else if(3.0 * p.y < m ) q = p.yzx;
    else if(3.0 * p.z < m ) q = p.zxy;
    else return m*0.57735027;

    float k = clamp(0.5 * (q.z - q.y + s), 0.0, s); 
    return length(vec3(q.x, q.y - s + k, q.z - k)); 
}

float sdBoundingBox(vec3 p, vec3 b, float e, float r) {
    p = abs(p) - b;
    vec3 q = abs(p + e) - e;
    return min(min(
        length(max(vec3(p.x, q.y, q.z), 0.0)) + min(max(p.x, max(q.y, q.z)), 0.0),
        length(max(vec3(q.x, p.y, q.z), 0.0)) + min(max(q.x, max(p.y, q.z)), 0.0)),
        length(max(vec3(q.x, q.y, p.z), 0.0)) + min(max(q.x, max(q.y, p.z)), 0.0)) -r;
}

float sdGyroid(vec3 p, float t, float b) {
    return abs(dot(sin(p), cos(p.zxy)) - b) - t;
}

float sdLineSeg(vec3 p, vec3 a, vec3 b, float w) {
    vec3 ap = p - a, ab = b - a;
    float t = clamp(dot(ap, ab) / dot(ab, ab), 0.0, 1.0);
    vec3 c = a + ab * t;
    return length(p - c) - w;
}

float displacement(vec3 p, vec3 m) {
    return sin(m.x * p.x) * sin(m.y * p.y) * sin(m.z * p.z);
}

float impulse(float x, float k) {
    float h = k * x;
    return h * exp(1.0 - h);
}

vec3 rotX(vec3 p, float a) {
    float s = sin(a);
    float c = cos(a);
    return vec3(
        p.x,
        c*p.y-s*p.z,
        s*p.y+c*p.z
    );
}

vec3 rotY(vec3 p, float a) {
    float s = sin(a);
    float c = cos(a);
    return vec3(
        c*p.x+s*p.z,
        p.y,
        -s*p.x+c*p.z
    );
}
 
vec3 rotZ(vec3 p, float a) {
    float s = sin(a);
    float c = cos(a);
    return vec3(
        c*p.x-s*p.y,
        s*p.x+c*p.y,
        p.z
    );
}

vec3 rot(vec3 p, vec3 a) {
    return rotX(rotY(rotZ(p, a.z), a.y), a.x);
}

vec3 twistX(vec3 p, float angle) {
    return rotX(p, p.x * angle);
}

vec3 twistY(vec3 p, float angle) {
    return rotY(p, p.y * angle);
}

vec3 twistZ(vec3 p, float angle) {
    return rotZ(p, p.z * angle);
}

vec3 translate(vec3 p, vec3 p1) {
    return p + (p1 * -1.0);
}

vec3 scale(vec3 p, float s) {
    vec3 p1 = p;
    p1 /= s;
    return p1;
} 

vec3Tuple repeat(vec3 p, vec3 size) {
    vec3 c = floor((p + size * 0.5 ) / size);
    vec3 path1 = mod(p + size * 0.5, size) - size * 0.5;
    return vec3Tuple(path1, c);
}

vec3Tuple repeatLimit(vec3 p, float c, vec3 size) {
    vec3 c1 = floor((p + size * 0.5 ) / size);
    vec3 path1 = p - c * clamp(round(p / c), -size, size);
    return vec3Tuple(path1, c1);
}

vec2Tuple repeatPolar(vec2 p, float repetitions) {
	float angle = 2 * PI / repetitions;
	float a = atan(p.y, p.x) + angle / 2.0;
	float r = length(p);
	float c = floor(a / angle);
	a = mod(a, angle) - angle / 2.0;
	vec2 path = vec2(cos(a), sin(a)) * r;
	// For an odd number of repetitions, fix cell index of the cell in -x direction
	// (cell index would be e.g. -5 and 5 in the two halves of the cell):
	if (abs(c) >= (repetitions / 2.0)) {
        c = abs(c);
    } 
	return vec2Tuple(path, vec2(c));
}

entity opUnion(entity m1, entity m2) {
    return m1.dist < m2.dist ? m1 : m2;
}

entity opSubtraction(entity m1, entity m2) {
    if(-m1.dist > m2.dist) {
        m1.dist *= -1.0;
        return m1;
    }
    else {
        return m2;
    }
    
}

entity opIntersection(entity m1, entity m2) {
    return m1.dist > m2.dist ? m1 : m2;
}

entity mCustom(vec3 path, float dist, material material) {
    entity m;
    m.dist = dist;
    m.point = path;
    m.material = material;
    return m;
}

entity mPlane(vec3 path, vec3 n, float h, float scale, material material) {
    entity m;
    vec3 p1 = path / scale;
    m.dist = sdPlane(p1, n, h) * scale;
    m.point = p1;
    m.material = material;
    return m;
}

entity mSphere(vec3 path, float radius, float scale, material material) {
    entity m;
    vec3 p1 = path / scale;
    m.dist = sdSphere(p1, radius) * scale;
    m.point = p1;
    m.material = material;
    return m;
}

entity mBox(vec3 path, vec3 size, float r, float scale, material material) {
    entity m;
    vec3 p1 = path / scale;
    m.dist = sdBox(p1, size, r) * scale;
    m.point = p1;
    m.material = material;
    return m;
}

entity mTorus(vec3 path, vec2 dim, float scale, material material) {
    entity m;
    vec3 p1 = path / scale;
    m.dist = sdTorus(p1, dim) * scale;
    m.point = p1;
    m.material = material;
    return m;
}

entity mCylinder(vec3 path, vec3 size, float r, float scale, material material) {
    entity m;
    vec3 p1 = path / scale;
    m.dist = sdCylinder(p1, size, r) * scale;
    m.point = p1;
    m.material = material;
    return m;
}

entity mCappedCylinder(vec3 path, vec2 size, float r, float scale, material material) {
    entity m;
    vec3 p1 = path / scale;
    m.dist = sdCappedCylinder(p1, size, r) * scale;
    m.point = p1;
    m.material = material;
    return m;
}

entity mHexagon(vec3 path, vec2 size, float r, float scale, material material) {
    entity m;
    vec3 p1 = path / scale;
    m.dist = sdHexagon(p1, size, r) * scale;
    m.point = p1;
    m.material = material;
    m.needNormals = true;
    return m;
}

entity mPyramid(vec3 path, float height, float scale, material material) {
    entity m;
    vec3 p1 = path / scale;
    m.dist = sdPyramid(p1, height) * scale;
    m.point = p1;
    m.material = material;
    return m;
}

entity mOctahedron(vec3 path, float height, float scale, material material) {
    entity m;
    vec3 p1 = path / scale;
    m.dist = sdOctahedron(p1, height) * scale;
    m.point = p1;
    m.material = material;
    return m;
}

entity mGyroid(vec3 path, float scale, float thickness, float bias, material material) {
    entity m;
    vec3 p1 = path / scale;
    m.dist = sdGyroid(p1, thickness, bias) * scale;
    m.point = p1;
    m.material = material;
    return m;
}

entity mLineSeg(vec3 path, vec3 a, vec3 b, float w, float scale, material material) {
    entity m;
    vec3 p1 = path / scale;
    m.dist = sdLineSeg(p1, a, b, w) * scale;
    m.point = p1;
    m.material = material;
    return m;
}

entity scene(vec3 path, vec2 uv) {       
    material torusM = material(
        ambientOptions(
            vec3(0.2, 0.2, 0.2),
            1.0
        ),
        diffuseOptions(
            vec3(0.2, 0.2, 0.2),
            1.0
        ),
        specularOptions(
            vec3(1.0, 1.0, 1.0),
            20.0,
            70.0
        ),
        shadowOptions(
            false,
            0.1,
            0.2,
            2.0,
            0.5
        ),
        aoOptions(
            false,
            1.0,
            1.0
        ),
        textureOptions(
            1,
            vec3(1.0, 1.0, 1.0),
            vec3(0.0, 0.0, 0.0),
            vec3(1.0, 1.0, 1.0)
        )
    );
    entity torus = mTorus(
        rot(path, vec3(1.0) * time),
        vec2(1.0, 0.5),
        1.0,
        torusM
    );
    torus.needNormals = true;

    material hexagonM = material(
        ambientOptions(
            vec3(0.2, 0.2, 0.2),
            1.0
        ),
        diffuseOptions(
            vec3(1.0, 1.0, 1.0),
            1.0
        ),
        specularOptions(
            vec3(1.0, 1.0, 1.0),
            20.0,
            70.0
        ),
        shadowOptions(
            false,
            0.1,
            0.2,
            2.0,
            0.5
        ),
        aoOptions(
            false,
            1.0,
            1.0
        ),
        textureOptions(
            0,
            vec3(1.0, 1.0, 1.0),
            vec3(0.0, 0.0, 0.0),
            vec3(1.0, 1.0, 1.0)
        )
    );
    entity hexagon = mHexagon(
        rot(translate(path, vec3(3.0, 0.0, 0.0)), vec3(1.0) * time),
        vec2(1.0, 0.5),
        0.0,
        1.0,
        hexagonM
    );
    hexagon.needNormals = true;

    material boxM = material(
        ambientOptions(
            vec3(0.2, 0.2, 0.2),
            1.0
        ),
        diffuseOptions(
            vec3(0.2, 0.2, 0.2),
            1.0
        ),
        specularOptions(
            vec3(0.2, 0.2, 0.2),
            20.0,
            70.0
        ),
        shadowOptions(
            false,
            0.1,
            0.2,
            2.0,
            0.5
        ),
        aoOptions(
            false,
            1.0,
            1.0
        ),
        textureOptions(
            2,
            vec3(1.0, 1.0, 1.0),
            vec3(0.0, 0.0, 0.0),
            vec3(1.0, 1.0, 1.0)
        )
    );
    entity box = mBox(
        rot(translate(path, vec3(-3.0, 0.0, 0.0)), vec3(1.0) * time),
        vec3(1.0, 1.0, 1.0),
        0.0,
        1.0,
        boxM
    );
    box.needNormals = true;

    material groundM = material(
        ambientOptions(
            vec3(0.2, 0.2, 0.2),
            1.0
        ),
        diffuseOptions(
            vec3(0.2, 0.2, 0.2),
            0.5
        ),
        specularOptions(
            vec3(1.0, 1.0, 1.0),
            0.0,
            0.0
        ),
        shadowOptions(
            true,
            0.1,
            2.0,
            0.001,
            16.0
        ),
        aoOptions(
            false,
            0.01,
            0.02
        ),
        textureOptions(
            0,
            vec3(1.0, 1.0, 1.0),
            vec3(0.0, 0.0, 0.0),
            vec3(1.0, 1.0, 1.0)
        )
    );
    entity ground = mPlane(
        translate(path, vec3(0.0, -2.0, 0.0)),
        vec3(0.0, 1.0, 0.0),
        1.0, 
        1.0,
        groundM
    );
    ground.needNormals = true;
    return opUnion(ground, opUnion(opUnion(hexagon, box), torus));
} 

vec3 calculatePointNormals(vec3 point, float threshold) {
    const vec2 k = vec2(1, -1);
    return normalize(
        k.xyy * scene(point + k.xyy * threshold, vec2(0)).dist + 
        k.yyx * scene(point + k.yyx * threshold, vec2(0)).dist + 
        k.yxy * scene(point + k.yxy * threshold, vec2(0)).dist + 
        k.xxx * scene(point + k.xxx * threshold, vec2(0)).dist
    );
}

hit raymarch(vec3 rayOrigin, vec3 rayDirection, vec2 uv) {
    hit h;
    for(float i = 0.0; i <= RAY_MAX_STEPS; i++) {
        vec3 point = rayOrigin + rayDirection * h.dist;
        h.entity = scene(point, uv);
        h.steps += 1.0;
        h.dist += h.entity.dist;
        if(abs(h.entity.dist) < RAY_THRESHOLD) {
            if(h.entity.needNormals == true) {
                h.entity.normal = calculatePointNormals(point, RAY_THRESHOLD);
            }
            break;

        }
        if(h.dist > RAY_MAX_DISTANCE) {
            break;
        }
    }
    
    return h;
}

float shadows(vec3 ro, vec3 lp, float mint, float maxt, float k) {
    float res = 1.0;
    float ph = 1e20;
    vec3 rd = normalize(ro - lp);
    for(float t = mint; t < maxt;)
    {
        float h = scene(ro + (rd * t), vec2(0)).dist;
        if(abs(h) < 0.001)
            return 0.0;
        float y = h * h / (2.0 * ph);
        float d = sqrt(h * h - y * y);
        res = min(res, k * d / max(0.0, t - y));
  
        ph = h;
        t += h;    
    }
    return res;
}

vec3 hsv2rgb(vec3 c) {
    vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix( vec3(1.0), rgb, c.y);
}

vec3 randomGeneratedTexture(vec3 position) {
    float d = length(position);
    vec3 color = hsv2rgb(vec3(time * 1.3 + position.z , 1.8, 1.4));
    return color;
}

vec4 textureCube(sampler2D sam, vec3 p, vec3 n)
{
	vec4 x = texture(sam, p.yz);
	vec4 y = texture(sam, p.zx);
	vec4 z = texture(sam, p.yx);
    vec3 a = abs(n);
	return (x* a.x + y * a.y + z * a.z) / (a.x + a.y + a.z);
}

vec3 background(vec2 uv, vec3 eye, vec3 rayDirection, hit h) {
    return vec3(1.0, 0.7, 0.0) * smoothstep(0.5, 1.3, uv.y);
}

float attenuation(vec3 lightPosition, vec3 position, float constant, float linear, float quadratic) {
    float distance = length(lightPosition - position);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance)); 
    return attenuation;
}

vec3 ambient(ambientOptions ambientOptions) {
    return ambientOptions.color * ambientOptions.strength;
} 

vec3 diffuse(vec3 normal, vec3 hit, vec3 lightDir, diffuseOptions diffuseOptions) {
    if (diffuseOptions.strength <= 0.0) {
        return vec3(0.0);
    }
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * diffuseOptions.color * diffuseOptions.strength;
    return diffuse;
}

vec3 specular(vec3 normal, vec3 hit, vec3 lightDir, vec3 eye, specularOptions specularOptions) {
    if (specularOptions.strength <= 0.0) {
        return vec3(0.0);
    }
    vec3 viewDir = normalize(eye - hit);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), specularOptions.shininess);
    vec3 specular = spec * specularOptions.strength * specularOptions.color;
    return specular;
} 

vec3 calculateOmniLight(vec3 normal, vec3 eye, vec3 lightPosition, vec3 hit, ambientOptions ambientOptions, diffuseOptions diffuseOptions, specularOptions specularOptions) {
    vec3 lightDir = normalize(lightPosition - hit);
    vec3 ambient = ambient(ambientOptions);
    vec3 diffuse = diffuse(normal, hit, lightDir, diffuseOptions);
    vec3 specular = specular(normal, hit, lightDir, eye, specularOptions);

    vec3 lights = (ambient + diffuse + specular);
    return lights;
}

vec3 calculatePointLights(vec3 normal, vec3 eye, pointLight light, vec3 hit, float attenuation, diffuseOptions diffuseOptions, specularOptions specularOptions) {
    vec3 lightDir = normalize(light.position - hit);
    vec3 diffuse = diffuse(normal, hit, lightDir, diffuseOptions);
    vec3 specular = specular(normal, hit, lightDir, eye, specularOptions);

    diffuse *= attenuation;
    specular *= attenuation;

    vec3 lights = (diffuse + specular) * (light.color * light.strength);
    return lights;
}

vec3 calculateShadows(vec3 origin, vec3 lightPos, shadowOptions shadowOptions) {
    if(shadowOptions.enabled == false) {
        return vec3(1.0);
    }
    float res = 1.0;
    float ph = 1e20;
    vec3 lightDir = normalize(origin + lightPos);
    for(float t = shadowOptions.lowerLimit; t < shadowOptions.upperLimit;)
    {
        float h = scene(origin + (lightDir * t), vec2(0)).dist;
        if(h < shadowOptions.limit)
            return vec3(0.0);
            
        float y = h * h / (2.0 * ph);
        float d = sqrt(h * h - y * y);
        res = min(res, shadowOptions.hardness * d / max(0.0, t - y));
  
        ph = h;
        t += h;    
    }
    return vec3(res);
}

vec3 ao(vec3 point, vec3 normal, aoOptions aoOptions) {
	float aoOut = 1.0;
	for (float i = 0.0; i < aoOptions.limit; i++) {
		aoOut -= (i * aoOptions.factor - scene(point + normal * i * aoOptions.factor, vec2(0.0)).dist) / pow(2.0, i);
	}
	return vec3(aoOut);
}

vec3 fog(vec3 original, vec3 color, float dist, float b) {
    return mix(original, color, 1.0 - exp(-dist * b));
}

vec3 generateTexture(vec3 point, textureOptions textureOptions) {
    vec3 r = textureOptions.baseColor;
    vec3 p = (point / textureOptions.scale) + textureOptions.offset;

    if (textureOptions.index == 1) 
        return randomGeneratedTexture(p);
    if (textureOptions.index == 2) 
        return textureCube(texture01, p, vec3(0.0, 0.0, 0.1)).xyz;
    return r;
}

vec3 determinePixelBaseColor(float steps, float dist) {
    float smoothedSteps = 1.0 - smoothstep(0.0, RAY_MAX_STEPS, steps);
    return vec3(smoothedSteps);
}

vec3 processColor(hit h, vec3 rd, vec3 eye, vec2 uv) { 
    if(h.dist > RAY_MAX_DISTANCE) {
        return background(uv, eye, rd, h);
    }

    entity entity = h.entity;
    material em = entity.material;
    vec3 base = determinePixelBaseColor(h.steps, h.dist); 
    vec3 texture = generateTexture(entity.point, em.texture);

    vec3 result = texture * base;
    if (entity.needNormals == true) {
        vec3 lights = calculateOmniLight(entity.normal, eye, omniLightPosition, entity.point, em.ambient, em.diffuse, em.specular);     
        vec3 shadows = calculateShadows(entity.point, omniLightPosition, em.shadow);
  
        for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++) {
            pointLight pLight = pointLights[i];
            float attenuation = attenuation(pLight.position, entity.point, pLight.constant, pLight.linear, pLight.quadratic);
            lights += calculatePointLights(entity.normal, eye, pLight, entity.point, attenuation, em.diffuse, em.specular);
        }

        lights *= shadows;
        result *= lights;

        if(em.ao.enabled)
        {
            result *= ao(entity.point, entity.normal, em.ao);
        }
    }      
    
    result = fog(result, fogColor, h.dist, fogIntensity);
    result = pow(result, vec3(1.0 / 1.2));
   
    return vec3(result);
}

vec3 drawMarching(vec2 uv) {
    vec3 forward = normalize(cameraLookAt - cameraPosition);   
    vec3 right = normalize(vec3(forward.z, 0.0, -forward.x));
    vec3 up = normalize(cross(forward, right)); 
    
    vec3 rayDirection = normalize(forward + cameraFov * uv.x * right + cameraFov * uv.y * up);
    hit marchHit = raymarch(cameraPosition, rayDirection, uv);
    return processColor(marchHit, rayDirection, cameraPosition, uv); 
}

void initialize() {
    pointLights[0].position = vec3(-2.0, 0.0, 0.0);
    pointLights[0].color = vec3(0.0, 0.0, 1.0);
    pointLights[0].strength = 6.0;
    pointLights[0].constant = 1.0;
    pointLights[0].linear = 0.027;
    pointLights[0].quadratic = 0.0028;

    pointLights[1].position = vec3(2.0, 0.0, 0.0);
    pointLights[1].color = vec3(0.0, 0.0, 1.0);;
    pointLights[1].strength = 6.0;
    pointLights[1].constant = 1.0;
    pointLights[1].linear = 0.027;
    pointLights[1].quadratic = 0.0028;  
}

void main() {
    float aspectRatio = resolution.x / resolution.y;
    vec2 uv = (gl_FragCoord.xy / resolution.xy) * 2.0 - 1.0;
    uv.x *= aspectRatio;

    initialize();
    FragColor = vec4(drawMarching(uv), 1.0);
}
