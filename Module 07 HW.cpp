#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>  // Для std::remove

using namespace std;

// Интерфейс для стратегии оплаты
class IPaymentStrategy {
public:
    virtual void pay(double amount) = 0;
    virtual ~IPaymentStrategy() = default;
};

// Реализация стратегии оплаты через карту
class CardPayment : public IPaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Оплата " << amount << " через карту." << endl;
    }
};

// Реализация стратегии оплаты через PayPal
class PayPalPayment : public IPaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Оплата " << amount << " через PayPal." << endl;
    }
};

// Реализация стратегии оплаты криптовалютой
class CryptoPayment : public IPaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Оплата " << amount << " криптовалютой." << endl;
    }
};

// Контекст для оплаты, использующий стратегии
class PaymentContext {
private:
    shared_ptr<IPaymentStrategy> paymentStrategy;

public:
    PaymentContext(shared_ptr<IPaymentStrategy> strategy) : paymentStrategy(strategy) {}

    void setPaymentStrategy(shared_ptr<IPaymentStrategy> strategy) {
        paymentStrategy = strategy;
    }

    void makePayment(double amount) {
        paymentStrategy->pay(amount);
    }
};

// Интерфейс для наблюдателя
class IObserver {
public:
    virtual void update(double exchangeRate) = 0;
    virtual ~IObserver() = default;
};

// Интерфейс для субъекта
class ISubject {
public:
    virtual void attach(shared_ptr<IObserver> observer) = 0;
    virtual void detach(shared_ptr<IObserver> observer) = 0;
    virtual void notify() = 0;
    virtual ~ISubject() = default;
};

// Реализация субъекта — курс обмена валют
class CurrencyExchange : public ISubject {
private:
    vector<shared_ptr<IObserver>> observers;
    double exchangeRate;

public:
    void setExchangeRate(double rate) {
        exchangeRate = rate;
        notify();
    }

    void attach(shared_ptr<IObserver> observer) override {
        observers.push_back(observer);
    }

    void detach(shared_ptr<IObserver> observer) override {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer), 
            observers.end()
        );
    }

    void notify() override {
        for (const auto& observer : observers) {
            observer->update(exchangeRate);
        }
    }
};

// Реализация наблюдателей
class BankObserver : public IObserver {
public:
    void update(double exchangeRate) override {
        cout << "Банк получил обновление курса: " << exchangeRate << endl;
    }
};

class StockMarketObserver : public IObserver {
public:
    void update(double exchangeRate) override {
        cout << "Фондовый рынок получил обновление курса: " << exchangeRate << endl;
    }
};

class ForexObserver : public IObserver {
public:
    void update(double exchangeRate) override {
        cout << "Форекс получил обновление курса: " << exchangeRate << endl;
    }
};

// Клиентский код
int main() {
    // Пример с паттерном "Стратегия"
    shared_ptr<IPaymentStrategy> cardPayment = make_shared<CardPayment>();
    shared_ptr<IPaymentStrategy> paypalPayment = make_shared<PayPalPayment>();
    shared_ptr<IPaymentStrategy> cryptoPayment = make_shared<CryptoPayment>();

    PaymentContext paymentContext(cardPayment);
    paymentContext.makePayment(100);

    paymentContext.setPaymentStrategy(paypalPayment);
    paymentContext.makePayment(200);

    paymentContext.setPaymentStrategy(cryptoPayment);
    paymentContext.makePayment(300);

    // Пример с паттерном "Наблюдатель"
    shared_ptr<CurrencyExchange> currencyExchange = make_shared<CurrencyExchange>();

    shared_ptr<IObserver> bank = make_shared<BankObserver>();
    shared_ptr<IObserver> stockMarket = make_shared<StockMarketObserver>();
    shared_ptr<IObserver> forex = make_shared<ForexObserver>();

    currencyExchange->attach(bank);
    currencyExchange->attach(stockMarket);
    currencyExchange->attach(forex);

    // Изменение курса валют
    currencyExchange->setExchangeRate(1.2);
    currencyExchange->setExchangeRate(1.3);

    // Удаление наблюдателя
    currencyExchange->detach(stockMarket);

    // Обновление после удаления
    currencyExchange->setExchangeRate(1.4);

    return 0;
}
