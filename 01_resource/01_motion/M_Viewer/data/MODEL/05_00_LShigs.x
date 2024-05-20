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
 9.78187;1.52108;-0.48072;,
 11.92309;35.58560;-3.38967;,
 4.05737;36.41855;5.81326;,
 1.63952;0.28767;15.06288;,
 -2.65641;36.97963;-1.02992;,
 -11.04141;0.76875;7.75078;,
 -17.26878;0.59388;-0.83960;,
 -10.24508;1.59655;-8.12157;,
 2.75249;36.17332;-8.21670;,
 1.57007;0.42132;-15.11492;,
 11.92309;35.58560;-3.38967;,
 9.78187;1.52108;-0.48072;;
 
 6;
 4;0,1,2,3;,
 4;3,2,4,5;,
 3;6,4,7;,
 4;7,4,8,9;,
 3;5,4,6;,
 4;9,8,10,11;;
 
 MeshMaterialList {
  1;
  6;
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MODEL\\samurai_Tex_New.png";
   }
  }
 }
 MeshNormals {
  8;
  -0.631102;0.235201;-0.739182;,
  0.826628;0.034291;0.561703;,
  -0.671838;0.258972;-0.693950;,
  -0.680839;0.302321;0.667129;,
  -0.709705;0.281637;-0.645755;,
  -0.575123;0.285253;0.766723;,
  -0.771994;0.312924;0.553267;,
  0.694068;-0.019861;-0.719635;;
  6;
  4;1,1,1,1;,
  4;5,5,3,3;,
  3;4,2,2;,
  4;2,2,0,0;,
  3;3,3,6;,
  4;7,7,7,7;;
 }
 MeshTextureCoords {
  12;
  0.506140;0.970170;,
  0.506140;0.678570;,
  0.411450;0.681210;,
  0.329190;0.999740;,
  0.347060;0.642840;,
  0.224530;0.953750;,
  0.112200;0.883550;,
  0.052670;0.773010;,
  0.305200;0.586120;,
  0.000260;0.679080;,
  0.300810;0.505210;,
  0.009230;0.508790;;
 }
}
