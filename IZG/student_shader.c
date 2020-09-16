/*!
 * @file
 * @brief This file contains implemenation of phong vertex and fragment shader.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <assert.h>
#include <math.h>

#include <student/gpu.h>
#include <student/student_shader.h>
#include <student/uniforms.h>
#include <stdio.h>

/// \addtogroup shader_side Úkoly v shaderech
/// @{

void phong_vertexShader(GPUVertexShaderOutput *const      output,
                        GPUVertexShaderInput const *const input,
                        GPU const                         gpu) {


  //handle to all uniforms
  Uniforms const uniformsHandle = gpu_getUniformsHandle(gpu);

  //getting uniform locations
  UniformLocation const viewMatrixLocation = getUniformLocation(gpu, "viewMatrix");
  UniformLocation const projectionMatrixLocation = getUniformLocation(gpu, "projectionMatrix");

  //getting pointers
  Mat4 const *const view = shader_interpretUniformAsMat4(uniformsHandle, viewMatrixLocation);
  Mat4 const *const proj = shader_interpretUniformAsMat4(uniformsHandle, projectionMatrixLocation);

  Vec3 const *const position = vs_interpretInputVertexAttributeAsVec3(gpu, input, 0);
  Vec3 const *const normala = vs_interpretInputVertexAttributeAsVec3(gpu, input, 1);

  //multiplications
  Mat4 mvp;
  Vec4 pos4;

  multiply_Mat4_Mat4(&mvp, proj, view);
  copy_Vec3Float_To_Vec4(&pos4, position, 1.f);
  multiply_Mat4_Vec4(&output->gl_Position, &mvp, &pos4);
  
  //putting data from input to output for fragment shader
  Vec3 *const position_pointer = vs_interpretOutputVertexAttributeAsVec3(gpu, output, 0);
  Vec3 *const normala_pointer = vs_interpretOutputVertexAttributeAsVec3(gpu, output, 1);

  init_Vec3(position_pointer,position->data[0],position->data[1],position->data[2]);
  init_Vec3(normala_pointer,normala->data[0],normala->data[1],normala->data[2]);
  
  
  /// \todo Naimplementujte vertex shader, který transformuje vstupní vrcholy do
  /// clip-space.<br>
  /// <b>Vstupy:</b><br>
  /// Vstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu ve
  /// world-space (vec3) a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).<br>
  /// <b>Výstupy:</b><br>
  /// Výstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu (vec3)
  /// ve world-space a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).
  /// Výstupní vrchol obsahuje pozici a normálu vrcholu proto, že chceme počítat
  /// osvětlení ve world-space ve fragment shaderu.<br>
  /// <b>Uniformy:</b><br>
  /// Vertex shader by měl pro transformaci využít uniformní proměnné obsahující
  /// view a projekční matici.
  /// View matici čtěte z uniformní proměnné "viewMatrix" a projekční matici
  /// čtěte z uniformní proměnné "projectionMatrix".
  /// Zachovejte jména uniformních proměnných a pozice vstupních a výstupních
  /// atributů.
  /// Pokud tak neučiníte, akceptační testy selžou.<br>
  /// <br>
  /// Využijte vektorové a maticové funkce.
  /// Nepředávajte si data do shaderu pomocí globálních proměnných.
  /// Pro získání dat atributů použijte příslušné funkce vs_interpret*
  /// definované v souboru program.h.
  /// Pro získání dat uniformních proměnných použijte příslušné funkce
  /// shader_interpretUniform* definované v souboru program.h.
  /// Vrchol v clip-space by měl být zapsán do proměnné gl_Position ve výstupní
  /// struktuře.<br>
  /// <b>Seznam funkcí, které jistě použijete</b>:
  ///  - gpu_getUniformsHandle()
  ///  - getUniformLocation()
  ///  - shader_interpretUniformAsMat4()
  ///  - vs_interpretInputVertexAttributeAsVec3()
  ///  - vs_interpretOutputVertexAttributeAsVec3()
  
}
//todo
void phong_fragmentShader(GPUFragmentShaderOutput *const      output,
                          GPUFragmentShaderInput const *const input,
                          GPU const                           gpu) {

//Uniform handle
Uniforms const uniformsHandle = gpu_getUniformsHandle(gpu);

//Locations
UniformLocation const cameraPositionLocation = getUniformLocation(gpu,"cameraPosition");
UniformLocation const lightPositionLocation = getUniformLocation(gpu,"lightPosition");

//converting to vec3
Vec3 const*const camera = shader_interpretUniformAsVec3(uniformsHandle, cameraPositionLocation);
Vec3 const*const light = shader_interpretUniformAsVec3(uniformsHandle, lightPositionLocation);
Vec3 const*const position_pointer = fs_interpretInputAttributeAsVec3(gpu, input, 0);
Vec3 const*const normala_pointer = fs_interpretInputAttributeAsVec3(gpu, input, 1);

//Vectors
Vec3 L;
Vec3 N;
Vec3 R;
Vec3 V;
Vec3 tmp;
Vec3 tmp2;
Vec3 combined;

//colors
Vec3 bunnyColor;
init_Vec3(&bunnyColor,0.f, 1.f, 0.f);
Vec3 lightColor;
init_Vec3(&lightColor,1.f,1.f,1.f);
float shine =40.f;

//light
sub_Vec3(&L, light, position_pointer);
normalize_Vec3(&L,&L);

//Normal
normalize_Vec3(&N, normala_pointer);

//Camera
sub_Vec3(&V, camera, position_pointer);
normalize_Vec3(&V,&V);

//diffuse light
float diffuse_light=dot_Vec3(&N,&L);
if(diffuse_light<0.f)
{
  diffuse_light=0.f;
}
if(diffuse_light>1.f)
{
  diffuse_light=1.f;
}

//Reflected light
multiply_Vec3_Float(&tmp, &V, -1.f);//otoceni kamery o 180 stupnu
reflect(&R, &tmp, &N);//preklopeni okolo normaly(odkud jde svetlo)
normalize_Vec3(&R,&R);

//specular light
float specular_light=dot_Vec3(&R,&L);
if(specular_light<0.f)
{
  specular_light=0.f;
}
if(specular_light>1.f)
{
  specular_light=1.f;
}

specular_light=pow(specular_light, shine);


multiply_Vec3_Float(&tmp,&bunnyColor, diffuse_light );
multiply_Vec3_Float(&tmp2,&lightColor,specular_light );
add_Vec3(&combined, &tmp, &tmp2);

  output->color.data[0]=combined.data[0];
  //printf("%f\n",output->color.data[0]=combined.data[0]);
  output->color.data[1]=combined.data[1];
  output->color.data[2]=combined.data[2];
  output->color.data[3]=1.f;
  



  /// \todo Naimplementujte fragment shader, který počítá phongův osvětlovací
  /// model s phongovým stínováním.<br>
  /// <b>Vstup:</b><br>
  /// Vstupní fragment by měl v nultém fragment atributu obsahovat
  /// interpolovanou pozici ve world-space a v prvním
  /// fragment atributu obsahovat interpolovanou normálu ve world-space.<br>
  /// <b>Výstup:</b><br>
  /// Barvu zapište do proměnné color ve výstupní struktuře.<br>
  /// <b>Uniformy:</b><br>
  /// Pozici kamery přečtěte z uniformní proměnné "cameraPosition" a pozici
  /// světla přečtěte z uniformní proměnné "lightPosition".
  /// Zachovejte jména uniformních proměnný.
  /// Pokud tak neučiníte, akceptační testy selžou.<br>
  /// <br>
  /// Dejte si pozor na velikost normálového vektoru, při lineární interpolaci v
  /// rasterizaci může dojít ke zkrácení.
  /// Zapište barvu do proměnné color ve výstupní struktuře.
  /// Shininess faktor nastavte na 40.f
  /// Difuzní barvu materiálu nastavte podle normály povrchu.
  /// V případě, že normála směřuje kolmo vzhůru je difuzní barva čistě bílá.
  /// V případě, že normála směřuje vodorovně nebo dolů je difuzní barva čiště zelená.
  /// Difuzní barvu spočtěte lineární interpolací zelené a bíle barvy pomocí interpolačního parameteru t.
  /// Interpolační parameter t spočtěte z y komponenty normály pomocí t = y*y (samozřejmě s ohledem na negativní čísla).
  /// Spekulární barvu materiálu nastavte na čistou bílou.
  /// Barvu světla nastavte na bílou.
  /// Nepoužívejte ambientní světlo.<br>
  /// <b>Seznam funkcí, které jistě využijete</b>:
  ///  - shader_interpretUniformAsVec3()
  ///  - fs_interpretInputAttributeAsVec3()
  
}

/// @}
