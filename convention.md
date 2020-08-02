# ��� ��Ģ

## ���� �̸�
��� ���ϰ� Ŭ���� ������ �̸��� Pascal Case�� �����Ѵ�.
```cpp
Main.cpp, Main.h, ...
```

## ���
��� ������ Ÿ�� ����� �빮�ڷ� �ۼ��ϸ� _�� �ܾ �����Ѵ�.
```cpp
const int MAX_USER_COUNT = 100;
```

## ���� ����
���� ���� �տ��� ���λ� G�� ���δ�.
```cpp
int Gvalue = 100;
```

## ��� ����
��� ��� ���� �� ���λ翡�� m�� ���̰� ���� PascalCase�� �����Ѵ�.
```cpp
public:
int mAge = 0;
bool mIsMale = false;
```

## �Լ� �̸�
�Լ� �̸��� Pascal Case�� �����Ѵ�.
```cpp
void TestFunction()
{
}
```

## ����ü
����ü ��� ��Ģ ���� Ŭ������ �����ϰ� ������.

# ������� �ۼ� ��Ģ

## Ŭ���� ���� ����

```cpp
// ���ø� ����� ��Ģ
// template Ű����� <> ���̿� ���� 1ĭ
// class Ű���� ��� typename Ű����� ������ ��
template <typename T>
class TypeName : public ParentTypeName
{
// 1. �ʿ�� static_assert

// 2. ��ũ�� ����
    UE4_MACROS
    OTHER_MACROS
    
// 3. friend Ŭ������ �ִٸ� ����
    friend class ����
    
// 4. �ش� class�� �������� Ÿ�Ժ�Ī�� �ʿ��ϴٸ�, ���� ���� �ռ� �̸� ����
private:
    using Ÿ�� ��Ī ����  
    enum, const
    
// 5. ������� ����
private:
    _memberVariableList; ...
    
// 6. ������/�Ҹ��� ����
public:
    TypeName();
    ~TypeName();
    
// 7. ���� ����Լ��� ����
public:
    static MemberFunctions(); ...
    
// 8. ���� ����Լ��� ����
public:
    virtual MemberFunctions(); ...
    
// 9. �Ϲ� ����Լ��� ����
public:
    Logical public Functions(); ...
    
// 10. getter/setter ����Լ��� ����
public:
    Getter/Setter Functions(); ...
};

// 11. �۷ι� �����̽� ����, �Լ��� ����
```

## ��� ���Ͽ����� �ٸ� ������� ������ �ִ��� �����ؾ� �Ѵ�

�������̽� ������ �ñ׳��Ŀ� �ʿ��� Ÿ�� ���� ������ �ִ��� ���漱��(forward declaration)�� Ȱ���Ѵ�.

���漱���� ����� �۷ι� �����̽��� �� ������, �ش� Ÿ�� �տ� ���̴� ����� �� �������� ��Ȳ�� �°� �����Ӱ� �����Ѵ�.

```cpp
// 1) ������ ���� �۷ι� �����̽����� ���漱���� �ϴ� �� OK
class UButton;
  
class TestProject: public UGsUIWindow
{
    GENERATED_BODY()
    
protected:
    UPROPERTY()
    class UButton* BtnOk;   // 2) Ÿ�� �տ� ���漱�� �������� ����ص� OK
```

# ��Ÿ ��Ģ

## NULL�� ���� �׸�, 'nullptr'�θ� ǥ���� ��

�𸮾� ������ �ڵ忡���� ������ NULL�� �����Ǳ� ������, ����ؼ� nullptr�� �����ϴ� ��

## ��� ������ '���� ��� �ʱ�ȭ'�ϴ� ������ ������
```cpp
UCLASS()
class TestProject : public UGsGameFlowBase
{
    GENERATED_BODY()
    
    UPROPERTY(Transient)
    UGsManagersGame*                mManagers            = nullptr;
  
    class GsStageManagerGame*      mStageManager        = nullptr;
    class GsContentsManagerGame*    mContentsGameManager = nullptr;
```
Lazy initialization�� �ʼ����� �༮���� �ƴ϶��, �̰� �׳� ������ ������.

## Primitive type�� ������ ����(�Լ�, ���ø� ���)�� ������� ����
(�Լ��� OUT �Ķ���ʹ� �翬�� �� �̾߱⿡ ���Ե��� ������ �̸� �����д�)

C++�� Primitive type���δ� ������ �͵��� �ִ�.

* Integral types
* Floating-point types
* Boolean
* char / wchar_t
* void
 
���� ���̴� �ڵ� �Ǽ� �� �ϳ��� "���縦 ���Ϸ��� (���)�����Ρ�"��� �Ϲݷ��̴� (�̴� ������ SharedPtr������ �����ϰ� �ݺ��Ǵµ�)

�ϴ� �� �Ϲݷ��� ũ�� ���鿡�� Primitive type�鿡 ���ؼ� �ƹ��� ȿ���� ����.

Primitive type���� ��� 8����Ʈ �̳��� ũ�⸦ ������, Byte-padding�� ���� �������� ���忡�� �׳� �� 8����Ʈ  (64��Ʈ ȯ�濡��)

�׷��� ���� ���� �ּҰ��� ���ϱ⿡ 8����Ʈ ũ�⸦ ������.

���, ũ�� ���鿡���� �ƹ��� �ǹ̰� ����

�Ƿ� ������ ������ ���� ������ ������.

�и� call by value�̸� ����� ���� call by reference�� �Ѱ� ����� ������ ���Եȴ�.
������ �Ѿ�� �� ���ó���� �� �ٸ� ���� ������ ����� �� �� �ִ�.
�и� �ѱ����� �Ѱ�µ�, �������� ������ �����Ǿ� ������ �� �ְ� �Ǵ� ���̴�.
 

�̴� ������ �������� ĸ��ȭ �ر��� �ǹ��ϱ⵵ �Ѵ�.

## '��Ȯ�� ����� �ο�'�� ��� �̻����� �߿��ϴ�
> ���� : https://docs.unrealengine.com/ko/Programming/Development/CodingStandard/#const%EC%A0%95%ED%99%95%EB%8F%84

�� �������� ���� �����ڵ���� �� �����Ǿ� �ֱ⿡, ���� �ڵ���� ��ũ�� ��ü

�߿��� ���� "Context ���� ���°� ������ �ʴ� �͵鿡 ���� constness�� �ο�"�ϰڴٴ� ������ �̸� ���� ������ ����� ������ ��!

1) �ڵ��� ������ ���� �� �ƴ϶�

2) �����Ϸ��� constness�� ����� �༮�鿡 ���� ����ȭ���ִ� �κ��� �������� ũ��.

## ���ڸ� ������ ������ �Լ��� '�ִ��� explicit' Ű����� �Բ�~

explicit�� ����� ������ ȣ���� ���� C++ language keyword�̴�.

�� ���� �������� explicit Ű���� ���� �����ڴ� �Ͻ���(implicit)���� ȣ��ȴٴ� �̾߱��̴�.

## 'auto' Ű����� Ư���� ��� ��찡 �ƴϸ� ������� �� ��

1. �����Լ��� ������ ������ �� ���

```cpp
int a = 7, b = 3;

// �ܺ� ���� a�� b�� ���� ĸ���ϰ�, int x�� �Ķ���ͷ� �޴� lambda �Լ�
// a * x + b �� ����� ����Ѵ�.
auto closureFunc = [a, b] (int x)
{
    cout << a * x + b << endl;
};
  
closureFunc(5);  // 7 * 5  + 3 = 38 �� ���
```


2. �������� iterator �� �ٷ� �� ���
```cpp
// iterator�� �ٷ� �� OK
for (auto ObjIt = Objects.CreateConstIterator(); ObjIt; ++ObjIt)
{
    // ...
}
```

3. Ranged base For Loop������ �κ� ��� ([key, value] �����̳ʸ� ���)
```cpp
// Ranged-For Loop�� �⺻������ NOT OK, ����ϸ� �ƴ� ��
// TArray<class UGsUIQuestSlotMain*> MainSlotList;
for (auto slot : MainSlotList)
// for (UGsUIQuestSlotMain* slot : MainSlotList)�� ���� ���
{
    if (slot)
    {
        slot->UpdateQuestDist();
    }
} 

// ������, [key, value]�� �ٷ�� ��쿣 �κ��� ���
// TMap<StatType, FGsStatInfo*> _statGroup;
for (auto& iter : _statGroup)
{
    delete iter.Value;
    iter.Value = nullptr;
}
```

�� 3������ ��������, �ٸ� ��� ���鿣 auto ����!

auto�� �������� ��������, auto Ű����� �ּ� ������ ���յǸ� �ڵ��� ��Ȯ���� ��������, ������ ����ų ���� �ְ� �ȴ�.
