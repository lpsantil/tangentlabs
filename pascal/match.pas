{-
  Another attempt to do type-based dispatch by way
  of try..except. It works once, but next time, it crashes. :/
-}
{$mode objfpc}{$macro on}
program match;
uses sysutils;

var MatchClass : TClass;
		   
{ macros to make the syntax work }
  {$define given  := try MatchClass := }
  {$define match  := .ClassType; raise MatchClass.InitInstance(MatchClass.NewInstance) except }
  {$define when   := on}

type
  TAnimal     = class end;
  TVegetable  = class end;
  TMineral    = class end;

var
  objects : array[0..2] of TObject;
  i	  : byte;
begin
  randomize;
  objects[0] := TAnimal.Create;
  objects[1] := TVegetable.Create;
  objects[2] := TMineral.Create;
  for i := 1 to 10 do
    given
      objects[ random(3) ]
    match
      when TAnimal    do writeln('animal!');
      when TVegetable do writeln('vegetable!');
      when TMineral   do writeln('mineral!');
    end
end.

{ example output:
  
  vegetable!
  animal!
  animal!
  animal!
  animal!
  animal!
  vegetable!
  animal!
  animal!
  vegetable!
}
