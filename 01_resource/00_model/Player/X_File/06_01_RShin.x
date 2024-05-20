xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 12;
 -2.43413;-0.00581;4.60000;,
 1.56587;-0.00581;-0.40000;,
 1.56587;10.99421;-5.40000;,
 -4.43413;10.99421;3.60000;,
 4.56587;10.99421;3.60000;,
 3.56587;-0.00581;4.60000;,
 -2.43413;-0.00581;4.60000;,
 1.56587;10.99421;-5.40000;,
 1.56587;-0.00581;-0.40000;,
 -4.43413;10.99421;3.60000;,
 4.56587;10.99421;3.60000;,
 3.56587;-0.00581;4.60000;;
 
 12;
 3;0,1,2;,
 3;0,2,3;,
 3;4,2,1;,
 3;4,1,5;,
 3;4,5,0;,
 3;4,0,3;,
 3;6,7,8;,
 3;6,9,7;,
 3;10,8,7;,
 3;10,11,8;,
 3;10,6,11;,
 3;10,9,6;;
 
 MeshMaterialList {
  1;
  12;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.097255;0.150588;0.279216;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  16;
  0.784685;0.235827;0.573284;,
  0.751173;0.273153;0.600938;,
  0.815623;0.197727;0.543749;,
  -0.931015;0.129881;0.341089;,
  -0.922061;0.117353;0.368824;,
  -0.939032;0.142277;0.313011;,
  0.000000;-0.090536;-0.995893;,
  0.000000;-0.090536;-0.995893;,
  -0.784685;-0.235827;-0.573284;,
  -0.751173;-0.273153;-0.600938;,
  -0.815623;-0.197727;-0.543749;,
  0.931015;-0.129881;-0.341089;,
  0.922061;-0.117353;-0.368824;,
  0.939032;-0.142277;-0.313011;,
  0.000000;0.090536;0.995893;,
  0.000000;0.090536;0.995893;;
  12;
  3;0,1,0;,
  3;0,0,2;,
  3;3,5,3;,
  3;3,3,4;,
  3;6,7,6;,
  3;6,6,7;,
  3;8,8,9;,
  3;8,10,8;,
  3;11,11,13;,
  3;11,12,11;,
  3;14,14,15;,
  3;14,15,14;;
 }
 MeshTextureCoords {
  12;
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
