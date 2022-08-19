#version 460 core

struct Material
{    
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular; 
       
    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
};

struct DataLight
{
    DirectionalLight directional_light;
    PointLight point_light;    
    vec2 tex_coord;
    vec3 camera_pos;
    vec3 normal;
    vec3 frag_pos;
};

struct Cone
{
    vec3 direction;
    float aperture;
    float weight;
};

void directional_light_func(in DataLight data, out vec3 result);
void point_light_func(in DataLight data, out vec3 result);
float directional_shadow_func(in vec4 light_frag_pos);
float omnidirectional_shadow_func(in vec3 frag_pos, in vec3 point_light_pos);
float VectorToDepthValue(vec3 Vec);
vec3 indirect_light_func(in vec4 voxel_frag_pos, in int directions);
vec3 cone_trace(in vec3 voxel_space_world_position, in vec3 direction, in float aperture, in float weight);

in vec3 frag_pos;
in vec3 normal;
in vec2 tex_coord;
in mat3 tbn_mat;
in vec4 light_frag_pos;
in vec4 voxel_frag_pos;

out vec4 frag_color;

uniform vec3 camera_pos;

uniform Material material;
uniform PointLight point_light;
uniform DirectionalLight dir_light;

uniform int directional_active;
uniform int indirect_light_active;
uniform int weight_active;
uniform int only_indirect_light_active;
uniform int only_indirect_light_high_contrast_active;
uniform int dark_places_help_active;
uniform float voxel_color_balance;

uniform float far_plane;
uniform float near_plane;
uniform int omnidirectional_shadow_enable;
uniform float omnidirectional_shadow_bias;

uniform float shadow_map_bias;
uniform int shadow_enable;

uniform int MAX_MIPMAP_LEVEL;
uniform float VOXEL_INITIAL_OFFSET;
uniform ivec3 VOXEL_DIMENSIONS;
uniform float VOXEL_SIZE;
uniform int DIRECTIONS;

uniform sampler2D texture_diffuse_0;
uniform sampler2D texture_diffuse_1;
uniform sampler2D texture_specular_0;
uniform sampler2D texture_specular_1;
uniform sampler2D texture_normal_0;
uniform sampler2D texture_normal_1;
uniform sampler2D directional_shadow_map;
uniform samplerCube omnidirectional_shadow_map;
uniform sampler3D voxel_map;

int current_dark_place = 0;

void main() 
{
    vec3 directional_result;
    vec3 point_result;
    vec3 indirect_light_result;
  
    DataLight data;
    data.directional_light = dir_light;
    data.point_light = point_light;
    data.tex_coord = tex_coord;
    data.camera_pos = camera_pos;    
    data.frag_pos = frag_pos;

    data.normal = texture(texture_normal_0, tex_coord).rgb;
    data.normal = data.normal * 2.0f - 1.0f;
    data.normal = normalize(tbn_mat * data.normal);

    directional_light_func(data, directional_result);
    point_light_func(data, point_result);

    if ((directional_result.r+directional_result.g+directional_result.b) < 0.05f)
        current_dark_place = 1;

    if (directional_active == 0 || only_indirect_light_active == 1)
    {
        directional_result = vec3(0.0f, 0.0f, 0.0f);
    }
    
    if (indirect_light_active == 1)
    {
        indirect_light_result = indirect_light_func(voxel_frag_pos, DIRECTIONS);

        if (only_indirect_light_high_contrast_active == 1)
        {
            indirect_light_result *= 5.0f;
        }

        // if (current_dark_place == 1)
        // {
        //     indirect_light_result *= (1.0f+dir_light.ambient);
        // }
    }

    frag_color = vec4(directional_result + indirect_light_result, 1.0f);
}

void directional_light_func(in DataLight data, out vec3 result)
{
    vec3 ambient = data.directional_light.ambient * texture(texture_diffuse_0, data.tex_coord).rgb;

    // negando porque o vetor direção da luz direcional é da luz em direção ao fragmento e não do fragmento a luz
    vec3 light_dir = normalize(-data.directional_light.direction); 
    vec3 diffuse = max(dot(light_dir, data.normal), 0.0f) * data.directional_light.diffuse * material.diffuse * texture(texture_diffuse_0, data.tex_coord).rgb;

    vec3 view_dir = normalize(data.camera_pos - data.frag_pos);
    vec3 reflect_dir = reflect(-light_dir, data.normal); // aqui negar denovo para não precisar normalizar o vetor original de novo
    vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess) * material.specular * texture(texture_specular_0, data.tex_coord).rgb * data.directional_light.specular;

    float shadow = 1.0f;
    if (shadow_enable == 1)
    {
        shadow = directional_shadow_func(light_frag_pos);
    }        

    if (indirect_light_active == 1)
    {
        ambient = vec3(0.0f);
    }

    result = ambient + shadow * (diffuse + specular);
}

void point_light_func(in DataLight data, out vec3 result)
{
    vec3 ambient = data.point_light.ambient * texture(texture_diffuse_0, data.tex_coord).rgb;

    vec3 light_dir = normalize(data.point_light.position - data.frag_pos);
    vec3 diffuse = max(dot(light_dir, data.normal), 0.0f) * data.point_light.diffuse * material.diffuse * texture(texture_diffuse_0, data.tex_coord).rgb;
    
    vec3 view_dir = normalize(data.camera_pos - data.frag_pos);
    vec3 reflect_dir = reflect(-light_dir, data.normal);    
    vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess) * material.specular * vec3(texture(texture_specular_0, data.tex_coord).r) * data.point_light.specular;

    float distance = length(data.point_light.position - data.frag_pos);
    float attenuation = 1.0f / (data.point_light.constant + data.point_light.linear * distance + data.point_light.quadratic * (distance * distance));

    result = (ambient * attenuation) + (diffuse * attenuation) + (specular * attenuation);
}

float directional_shadow_func(in vec4 light_frag_pos)
{
    light_frag_pos = light_frag_pos / light_frag_pos.w;
    light_frag_pos = light_frag_pos * 0.5f + 0.5f;
    
    if (light_frag_pos.z > 1.0f)
    {
        light_frag_pos.z = 1.0f;
    }

    float depth = texture(directional_shadow_map, light_frag_pos.xy).r;

    if ((depth + shadow_map_bias) < light_frag_pos.z)
    {
        return 0.0f;
    } 
    else
    {
        return 1.0f;
    }    
}

float omnidirectional_shadow_func(in vec3 frag_pos, in vec3 point_light_pos)
{
    vec3 light_direction = frag_pos.xyz - point_light_pos;
    float cur_distance = VectorToDepthValue(light_direction);

    float closest_distance = texture(omnidirectional_shadow_map, light_direction).r;

    if ((closest_distance + omnidirectional_shadow_bias) < cur_distance)
    {
        return 0.0f;
    }
    else 
    {
        return 1.0f;
    }
}

float VectorToDepthValue(vec3 Vec)
{
    vec3 AbsVec = abs(Vec);
    float LocalZcomp = max(AbsVec.x, max(AbsVec.y, AbsVec.z));

    const float f = far_plane;
    const float n = near_plane;
    float NormZComp = (f+n) / (f-n) - (2*f*n)/(f-n)/LocalZcomp;
    return (NormZComp + 1.0) * 0.5;
}

vec3 indirect_light_func(in vec4 voxel_frag_pos, in int directions)
{
    vec3 voxel_space_world_pos = voxel_frag_pos.xyz / voxel_frag_pos.w;
    vec3 irradiance = vec3(0.0f);
    Cone directions_pool[9];

    if (directions == 9)
    {
        directions_pool[0] = Cone(vec3(0.33f, 0.8f, 0.5f), 0.4142f, 1.0f); // direction, tan of 22.5 degrees half aperture in radians
        directions_pool[1] = Cone(vec3(0.8f, 0.33f, 0.5f), 0.4142f, 1.0f);
        directions_pool[2] = Cone(vec3(0.8f, -0.33f, 0.5f), 0.4142f, 1.0f);
        directions_pool[3] = Cone(vec3(0.33f, -0.8f, 0.5f), 0.4142f, 1.0f);
        directions_pool[4] = Cone(vec3(-0.33f, -0.8f, 0.5f), 0.4142f, 1.0f);
        directions_pool[5] = Cone(vec3(-0.8f, -0.33f, 0.5f), 0.4142f, 1.0f);
        directions_pool[6] = Cone(vec3(-0.8f, 0.33f, 0.5f), 0.4142f, 1.0f);
        directions_pool[7] = Cone(vec3(-0.33f, 0.8f, 0.5f), 0.4142f, 1.0f);
        directions_pool[8] = Cone(vec3(0.0f, 0.0f, 1.0f), 0.75f, 1.0f); // direction, tan of 36.86 degress half aperture in radians        
    }

    else if (directions == 6)
    {
        directions_pool[0] = Cone(vec3(0.0f, 0.0f, 1.0f), 0.577f, 1.0f);
        directions_pool[1] = Cone(vec3(0.0f, 0.866025f, 0.5f), 0.577f, 1.0f);
        directions_pool[2] = Cone(vec3(0.823639f, 0.267617f, 0.5f), 0.577f, 1.0f);
        directions_pool[3] = Cone(vec3(0.509037f, -0.700629f, 0.5f), 0.577f, 1.0f);
        directions_pool[4] = Cone(vec3(-0.509037f, -0.700629f, 0.5f), 0.577f, 1.0f);
        directions_pool[5] = Cone(vec3(-0.823639f, 0.267617f, 0.5f), 0.577f, 1.0f);
    }

    else if (directions == 5)
    {
        directions_pool[0] = Cone(vec3(0.0f, 0.0f, 0.1f), 0.577f, 0.75);
        directions_pool[1] = Cone(vec3(0.5f, 0.0f, 0.87f), 0.4142f, 0.125);
        directions_pool[2] = Cone(vec3(0.0f, 0.5f, 0.87f), 0.4142f, 0.125);
        directions_pool[3] = Cone(vec3(0.0f, -0.5f, 0.87f), 0.4142f, 0.125);
        directions_pool[4] = Cone(vec3(-0.5f, 0.0f, 0.87f), 0.4142f, 0.125);
    }

    else if (directions == 1)
    {
        directions_pool[0] = Cone(vec3(0.0f, 0.0f, 0.1f), 0.4142f, 1);
    }

    // fragment local coordinate system
    // verificar se o vetor não é um dos vetores da base canônica
    vec3 u = vec3(0.0f, 1.0f, 0.0f);
    if (abs(dot(normal, u)) > 0.99999f)
    {
        u = vec3(0.000000, 0.939693, 0.342020);
    }
    vec3 tangent = normalize(cross(normal, u));
    vec3 bitangent = normalize(cross(tangent, normal));
    
    vec3 cur_direction;
    for (int i = 0; i < directions; i += 1)
    {
        cur_direction = directions_pool[i].direction;
        // cur_direction = cur_direction.x * tangent + cur_direction.y * normal + cur_direction.z * bitangent;

        cur_direction = normalize(tbn_mat * directions_pool[i].direction);
        irradiance += cone_trace(voxel_space_world_pos, cur_direction, directions_pool[i].aperture, directions_pool[i].weight);
    }

    irradiance = irradiance * texture(texture_diffuse_0, tex_coord).rgb;
        
    return irradiance;
}

vec3 cone_trace(in vec3 voxel_space_world_position, in vec3 direction, in float aperture, in float weight)
{
    if (weight_active != 1)
    {
        weight = 1;
    }
    
    vec4 radiance = vec4(0.0f);

	float step = VOXEL_INITIAL_OFFSET * (VOXEL_SIZE/(VOXEL_DIMENSIONS.x)); // initial offset so the voxel
    
    // voxel_space_world_position = voxel_space_world_position + (tbn_mat * normal) * (step + 5 * step);

    while(radiance.a == 0.0f)
    {
        float diameter = 2 * aperture * step; // aperture already have the tan(radians(aperture)) computed        
        float mipmap_level = log2(diameter / (VOXEL_SIZE/VOXEL_DIMENSIONS.x));
        
        if (mipmap_level > MAX_MIPMAP_LEVEL)
        {
            break;
        }
        
        vec3 position = voxel_space_world_position + step * direction;

        // ensuring the frag position in voxel space is inside the ndc volume
        if (abs(position.x) > 1.0f || abs(position.y) > 1.0f || abs(position.z) > 1.0f)
        {
            break;
        }

     	vec3 vox_tex_coord = position * 0.5f + 0.5f;
     	vec4 sampling = textureLod(voxel_map, vox_tex_coord, mipmap_level);
        sampling = sampling / voxel_color_balance;
    	
        // alpha blending.
     	radiance.rgb += (1.0 - radiance.a) * sampling.a * sampling.rgb;
     	radiance.a += (1.0 - radiance.a) * sampling.a;
     	
     	step += diameter;
    }

    return (weight * radiance.rgb);
}