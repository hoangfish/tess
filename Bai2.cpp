#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class Circuit
{
public:
    virtual double totalResistance() const = 0;
    virtual void print(int indent = 0) const = 0;
    virtual ~Circuit() = default;
};

class Resistor : public Circuit
{
private:
    string name;
    double resistance;

public:
    Resistor(const string &n, double r);
    double totalResistance() const override;
    void print(int indent = 0) const override;
};

Resistor::Resistor(const string &n, double r) : name(n), resistance(r) {}

double Resistor::totalResistance() const
{
    return resistance;
}

void Resistor::print(int indent) const
{
    cout << string(indent, ' ') << name << ": " << resistance << " ohms\n";
}

class SeriesCircuit : public Circuit
{
private:
    vector<shared_ptr<Circuit>> components;

public:
    void add(const shared_ptr<Circuit> &c);
    double totalResistance() const override;
    void print(int indent = 0) const override;
};

void SeriesCircuit::add(const shared_ptr<Circuit> &c)
{
    components.push_back(c);
}

double SeriesCircuit::totalResistance() const
{
    double total = 0;
    for (const auto &c : components)
    {
        total += c->totalResistance();
    }
    return total;
}

void SeriesCircuit::print(int indent) const
{
    cout << string(indent, ' ') << "SeriesCircuit:\n";
    for (const auto &c : components)
    {
        c->print(indent + 2);
    }
}

class ParallelCircuit : public Circuit
{
private:
    vector<shared_ptr<Circuit>> components;

public:
    void add(const shared_ptr<Circuit> &c);
    double totalResistance() const override;
    void print(int indent = 0) const override;
};

void ParallelCircuit::add(const shared_ptr<Circuit> &c)
{
    components.push_back(c);
}

double ParallelCircuit::totalResistance() const
{
    double inverse = 0;
    for (const auto &c : components)
    {
        inverse += 1.0 / c->totalResistance();
    }
    return (inverse != 0) ? 1.0 / inverse : 0;
}

void ParallelCircuit::print(int indent) const
{
    cout << string(indent, ' ') << "ParallelCircuit:\n";
    for (const auto &c : components)
    {
        c->print(indent + 2);
    }
}

int main()
{
    auto r1 = make_shared<Resistor>("R1", 100);
    auto r2 = make_shared<Resistor>("R2", 200);
    auto r3 = make_shared<Resistor>("R3", 150);
    auto r4 = make_shared<Resistor>("R4", 300);
    auto r5 = make_shared<Resistor>("R5", 600);

    auto r1_r2_parallel = make_shared<ParallelCircuit>();
    r1_r2_parallel->add(r1);
    r1_r2_parallel->add(r2);

    auto r123_series = make_shared<SeriesCircuit>();
    r123_series->add(r1_r2_parallel);
    r123_series->add(r3);

    auto r4_r5_parallel = make_shared<ParallelCircuit>();
    r4_r5_parallel->add(r4);
    r4_r5_parallel->add(r5);

    auto fullCircuit = make_shared<SeriesCircuit>();
    fullCircuit->add(r123_series);
    fullCircuit->add(r4_r5_parallel);

    fullCircuit->print();
    cout << "Total Resistance: " << fullCircuit->totalResistance() << " ohms\n";

    return 0;
}
