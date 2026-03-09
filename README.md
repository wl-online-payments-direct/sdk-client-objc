# SDK Decommissioned

**Online Payments Objective-C SDK has been decommissioned. Please use our [Swift SDK](https://github.com/wl-online-payments-direct/sdk-client-swift) instead.**

# Online Payments Objective-C SDK

The Online Payments Objective-C SDK helps you with accepting payments in your iOS app, supporting iOS 9.0 and up, through the Online Payments platform.

The SDK's main function is to establish a secure channel between your iOS app and our server. This channel processes security credentials to guarantee the safe transit of your customers' data during the payment process.

**The Online Payments SDK helps you with:**
- Handling encryption of the payment context
- Convenient Objective-C wrappers for API responses
- User-friendly formatting of payment data, such as card numbers and expiry dates
- Validating user input
- Determining the card's associated payment provider

## Table of Contents

- [Online Payments Objective-C SDK](#online-payments-objective-c-sdk)
  - [Table of Contents](#table-of-contents)
  - [Installation](#installation)
    - [CocoaPods](#cocoapods)
    - [Carthage](#carthage)
  - [Example app](#example-app)
  - [Getting started](#getting-started)
  - [Type definitions](#type-definitions)
    - [OPSession](#opsession)
      - [Logging of requests and responses](#logging-of-requests-and-responses)
    - [OPPaymentContext](#oppaymentcontext)
    - [OPPaymentItems](#oppaymentitems)
    - [OPBasicPaymentProduct](#opbasicpaymentproduct)
    - [OPAccountOnFile](#opaccountonfile)
    - [OPPaymentProduct](#oppaymentproduct)
    - [OPPaymentProductField](#oppaymentproductfield)
    - [OPPaymentRequest](#oppaymentrequest)
      - [Tokenize payment request](#tokenize-payment-request)
      - [Set field values to payment request](#set-field-values-to-payment-request)
      - [Validate payment request](#validate-payment-request)
      - [Encrypt payment request](#encrypt-payment-request)
    - [IINDetails](#iindetails)
    - [OPStringFormatter](#opstringformatter)
  - [Payment Steps](#payment-steps)
    - [1. Initialize the Objective-C SDK for this payment](#1-initialize-the-objective-c-sdk-for-this-payment)
    - [2. Retrieve the payment items](#2-retrieve-the-payment-items)
    - [3. Retrieve payment product details](#3-retrieve-payment-product-details)
    - [4. Encrypt payment information](#4-encrypt-payment-information)
    - [5. Response from the Server API call](#5-response-from-the-server-api-call)

## Installation

The Online Payments Objective-C SDK is available via the following package managers: [CocoaPods](https://cocoapods.org/) or [Carthage](https://github.com/Carthage/Carthage).

### CocoaPods

You can add the Objective-C SDK as a pod to your project by adding the following to your `Podfile`:

```
$ pod 'OnlinePaymentsSDK'
```

Afterwards, run the following command:

```
$ pod install
```

### Carthage

You can add the Objective-C SDK with Carthage, by adding the following to your `Cartfile`:

```
$ github "online-payments/sdk-client-objc"
```

Afterwards, run the following command:

```
$ carthage update --use-xcframeworks
```

Navigate to the ```Carthage/Build``` directory, which was created in the same directory as where the ```.xcodeproj``` or ```.xcworkspace``` is. Inside this directory the ```.xcframework``` bundle is stored. Drag the ```.xcframework``` into the "Framework, Libraries and Embedded Content" section of the desired target. Make sure that it is set to "Embed & Sign".

## Example app

For your convenience, we also provide an example app that can be used as a basis for your own implementation. If you are fine with the look-and-feel of the example app, you do not need to make any changes at all. The example app can be found [here](https://github.com/Online-Payments/sdk-client-objc-example). Please note that the Objective-C example uses the Swift SDK, so you might need to make a few minor adjustments before the example works with the Objective-C SDK.

## Getting started

To accept your first payment using the SDK, complete the steps below. Also see the section [Payment Steps](#payment-steps) for more details on these steps.

1. Request your server to create a Client Session, using one of our available Server SDKs. Return the session details to your app.
2. Initialize the SDK using the session details.
```objective-c
OPSession *session = [OPSession sessionWithClientSessionId:clientSessionId customerId:customerId baseURL:baseURL assetBaseURL:assetsBaseURL appIdentifier:@"objc Example Application/v1.0.0"];
```
3. Configure your payment context.
```objective-c
// amount in cents & three letter currency code as defined in ISO 4217
OPPaymentAmountOfMoney *amountOfMoney = [[OPPaymentAmountOfMoney alloc] initWithTotalAmount:1298 currencyCode:@"EUR"];

// two letter country code as defined in ISO 3166-1 alpha-2
OPPaymentContext *paymentContext = [[OPPaymentContext alloc] initWithAmountOfMoney:1298 isRecurring:NO countryCode:@"EUR"];
```
4. Retrieve the available Payment Products. Display the `OPBasicPaymentItem` and `OPAccountOnFile` lists and request your customer to select one.
```objective-c
[self.session
    paymentItemsForContext:paymentContext
    groupPaymentProducts:NO
    success:^(OPPaymentItems *paymentItems) {
        // Display the contents of paymentItems & accountsOnFile to your customer
    }
    failure:^(NSError *error) {
        // Inform the customer that something went wrong while retrieving the available Payment Products
    }
];
```
5. Once the customer has selected the desired payment product, retrieve the enriched `OPPaymentProduct` detailing what information the customer needs to provide to authorize the payment. Display the required information fields to your customer.
```objective-c
[self.session
    paymentProductWithId:@"1"
    context:paymentContext
    success:^(OPPaymentProduct *paymentProduct) {
        // Display the fields to your customer
    }
    failure:^(NSError *error) {
        // Handle failure of retrieving a Payment Product by id
    }
];
```
6. Save the customer's input for the required information fields in a `OPPaymentRequest`.
```objective-c
OPPaymentRequest *request = [[OPPaymentRequest alloc] init];

[paymentRequest setValue:@"1245 1254 4575 45" forField:@"cardNumber"];
[paymentRequest setValue:@"123" forField:@"cvv"];
[paymentRequest setValue:@"12/25" forField:@"expiryDate"];
```
7. Validate and encrypt the payment request. The encrypted customer data should then be sent to your server.
```objective-c
[self.session
    preparePaymentRequest:paymentRequest
    success:^(OPPreparedPaymentRequest *preparedPaymentRequest) {
        // Forward the encryptedFields to your server
    }
    failure:^(NSError *error) {
        // Handle failure of encrypting Payment Request
    }
];
```
8. Request your server to create a payment request, using the Server API's Create Payment call. Provide the encrypted data in the `encryptedCustomerInput` field.

## Type definitions
### OPSession

For all interactions with the SDK an instance of `OPSession` is required. The following code fragment shows how `OPSession` is initialized. The session details are obtained by performing a Create Client Session call using the Server API.
```objective-c
OPSession *session = [OPSession sessionWithClientSessionId:clientSessionId customerId:customerId baseURL:baseURL assetBaseURL:assetsBaseURL appIdentifier:@"objc Example Application/v1.0.0" loggingEnabled:YES];
```
Almost all methods that are offered by `OPSession` are simple wrappers around the Client API. They make the request and convert the response to Objective-C objects that may contain convenience functions.

#### Logging of requests and responses
You are able to log requests made to the server and responses received from the server. By default logging is disabled, and it is important to always disable it in production. You are able to enable the logging in two ways. Either by setting its value when creating a Session - as shown in the code fragment above - or by setting its value after the Session was already created.
```objective-c
session.loggingEnabled = YES;
```

### OPPaymentContext

`OPPaymentContext` is an object that contains the context/settings of the upcoming payment. It is required as an argument to some of the methods of the `OPSession` instance. This object can contain the following details:
```objective-c
@interface OPPaymentContext

@property (strong, nonatomic) OPPaymentAmountOfMoney *amountOfMoney; // contains the total amount and the ISO 4217 currency code
@property (nonatomic, readonly) BOOL isRecurring; // Set `true` when payment is recurring. Default false.
@property (strong, nonatomic, readonly) NSString *countryCode; // ISO 3166-1 alpha-2 country code
@property (strong, nonatomic) NSString *locale; // IETF Language Tag + ISO 15897, example: 'nl_NL'
```

### OPPaymentItems
This object contains the available Payment Items for the current payment. Use the `[session paymentItemsForContext]` function to request the data.

The object you will receive is `OPPaymentItems`, which contains two lists. One for all available `OPBasicPaymentItem`s and one for all available `OPAccountOnFile`s. 

The code fragment below shows how to get the `OPPaymentItems` instance.
```objective-c
[self.session
    paymentItemsForContext:paymentContext
    groupPaymentProducts:NO
    success:^(OPPaymentItems *paymentItems) {
        // Display the contents of paymentItems & accountsOnFile to your customer
    }
    failure:^(NSError *error) {
        // Inform the customer that something went wrong while retrieving the available Payment Products
    }
];
```

### OPBasicPaymentProduct

The SDK offers two types to represent information about payment products:
`OPBasicPaymentProduct` and `OPPaymentProduct`. Practically speaking, instances of `OPBasicPaymentProduct` contain only the information that is required to display a simple list of payment products from which the customer can select one.

The type `OPPaymentProduct` contains additional information, such as the specific form fields that the customer is required to fill out. This type is typically used when creating a form that asks the customer for their details. See the [OPPaymentProduct](#oppaymentproduct) section for more info.

Below is an example for how to obtain display names and assets for the Visa product.
```objective-c
NSObject<OPBasicPaymentItem> *basicPaymentProduct = [self.paymentItems paymentItemWithIdentifier:@"1"];

NSString *identifier = basicPaymentProduct.identifier; // 1
NSString *label = [basicPaymentProduct.displayHintsList firstObject].label; // VISA
NSString *logoPath = [basicPaymentProduct.displayHintsList firstObject].logoPath; // https://assets.com/path/to/visa/logo.gif
```

### OPAccountOnFile

An instance of `OPAccountOnFile` represents information about a stored card product for the current customer. `OPAccountOnFile` IDs available for the current payment must be provided in the request body of the Server API's Create Client Session call.  If the customer wishes to use an existing `OPAccountOnFile` for a payment, the selected `OPAccountOnFile` should be added to the `OPPaymentRequest`. The code fragment below shows how display data for an account on file can be retrieved. This label can be shown to the customer, along with the logo of the corresponding payment product.
```objective-c
// All available accounts on file for the payment product
 NSArray<OPAccountOnFile *> *allAccountsOnFile = basicPaymentProduct.accountsOnFile.accountsOnFile;

// Get specific account on file for the payment product
OPAccountOnFile *accountOnFile = [basicPaymentProduct accountOnFileWithIdentifier:@"123"];

// Shows a mask based formatted value for the obfuscated cardNumber.
// The mask that is used is defined in the displayHints of this accountOnFile
// If the mask is {{9999}} {{9999}} {{9999}} {{9999}}, then the result would be **** **** **** 7412
NSString *maskedValue = [accountOnFile maskedValueForField:@"cardNumber"];
```

### OPPaymentProduct

`OPBasicPaymentProduct` only contains the information required by a customer to distinguish one payment product from another. However, once a payment product or an account on file has been selected, the customer must provide additional information, such as a bank account number, a credit card number, or an expiry date, before a payment can be processed. Each payment product can have several fields that need to be completed to process a payment. Instances of `OPBasicPaymentProduct` do not contain any information about these fields.

Information about the fields of payment products are represented by instances of `OPPaymentProductField`, which are contained in instances of `OPPaymentProduct`. The class `OPPaymentProductField` is described further down below. The `OPSession` instance can be used to retrieve instances of `OPPaymentProduct`, as shown in the following code fragment.
```objective-c
[self.session
    paymentProductWithId:@"1"
    context:paymentContext
    success:^(OPPaymentProduct *paymentProduct) {
        // Display the fields to your customer
    }
    failure:^(NSError *error) {
        // Handle failure of retrieving a Payment Product by id
    }
];
```

### OPPaymentProductField

The fields of payment products are represented by instances of `OPPaymentProductField`. Each field has an identifier, a type, a definition of restrictions that apply to the value of the field, and information about how the field should be presented graphically to the customer. Additionally, an instance of a field can be used to determine whether a given value is valid for the field.

In the code fragment below, the field with identifier `"cvv"` is retrieved from a payment product. The data restrictions of the field are inspected to see whether the field is a required field or an optional field. Additionally, the display hints of the field are inspected to see whether the values a customer provides should be obfuscated in a user interface.
```objective-c
OPPaymentProductField *cvvField = [paymentProduct paymentProductFieldWithId:@"cvv"];

BOOL isRequired = cvvField.dataRestrictions.isRequired; // state if value is required for this field
BOOL shouldObfuscate = cvvField.displayHints.obfuscate; // state if field value should be obfuscated
```

### OPPaymentRequest

Once a payment product has been selected and an instance of `OPPaymentProduct` has been retrieved, a payment request can be constructed. This class must be used as a container for all the values the customer provides.
```objective-c
OPPaymentRequest *paymentRequest = [[OPPaymentRequest alloc] init];
paymentRequest.paymentProduct = paymentProduct;
```

#### Tokenize payment request
A `OPPaymentProduct` has a property `tokenize`, which is used to indicate whether a payment request should be stored as an account on file. The code fragment below shows how a payment request should be constructed when the request should *not* be stored as an account on file. By default, `tokenize` is set to `false`.
```objective-c
OPPaymentRequest *paymentRequest = [[OPPaymentRequest alloc] init]; // tokenize is false by default
paymentRequest.paymentProduct = paymentProduct;

// you can set the request's tokenize property after having initialized the paymentRequest
paymentRequest.tokenize = YES;
```

If the customer selected an account on file, both the account on file and the corresponding payment product must be supplied while constructing the payment request, as shown in the code fragment below. Instances of `OPAccountOnFile` can be retrieved from instances of `OPBasicPaymentProduct` and `OPPaymentProduct`.
```objective-c
OPPaymentRequest *paymentRequest = [[OPPaymentRequest alloc] init]; // accountOnFile is nil by default
paymentRequest.paymentProduct = paymentProduct;

// you can set the request's accountOnFile property after having initialized the paymentRequest
paymentRequest.accountOnFile = accountOnFile;
```

#### Set field values to payment request

Once a payment request has been configured, the value for the payment product's fields can be supplied as shown below. The identifiers of the fields, such as "cardNumber" and "cvv" in the example below, are used to set the values of the fields using the payment request.
```objective-c
[paymentRequest setValue:@"1245 1254 4575 45" forField:@"cardNumber"];
[paymentRequest setValue:@"123" forField:@"cvv"];
[paymentRequest setValue:@"12/25" forField:@"expiryDate"];
```

#### Validate payment request

Once all values have been supplied, the payment request can be validated. Behind the scenes the validation uses the `OPDataRestrictions` class for each of the fields that were added to the `OPPaymentRequest`. After the validation, a list of errors is available, which indicates any issues that have occured during validation. If there are no errors, the payment request can be encrypted and sent to our platform via your server. If there are validation errors, the customer should be provided with feedback about these errors.
```objective-c
// validate all fields in the payment request
[paymentRequest validate];

// check if the payment request is valid
if (paymentRequest.errors.count == 0) {
    // payment request is valid
} else {
    // payment request has errors
}
```

The validation errors are the `OPValidator`s linked to the `OPPaymentProductField` and are returned as a value, for example:
```objective-c
for (OPValidationError *error in paymentRequest.errors) {
    // do something with the error, like displaying it to the user
}
```

#### Encrypt payment request

The `OPPaymentRequest` is ready for encryption once the `OPPaymentProduct` is set, the `OPPaymentProductField` values have been provided and validated, and potentially the selected `OPAccountOnFile` or `tokenize` properties have been set. The `OPPaymentRequest` encryption is done by using `[session preparePaymentRequest]`. This will return a `OPPreparedPaymentRequest` which contains the encrypted payment request fields and encoded client meta info.
```objective-c
[self.session
    preparePaymentRequest:paymentRequest
    success:^(OPPreparedPaymentRequest *preparedPaymentRequest) {
        // Pass the encrypted payment request to your server which should then forward it to the Server API
    }
    failure:^(NSError *error) {
        // Handle failure of encrypting a Payment Request
    }
];
```

> Although it is possible to use your own encryption algorithms to encrypt a payment request, we advise you to use the encryption functionality that is offered by the SDK.

### IINDetails

The first six digits of a payment card number are known as the *Issuer Identification Number (IIN)*. As soon as the first 6 digits of the card number have been captured, you can use the `[session IINDetailsForPartialCreditCardNumber]` call to retrieve the payment product and network that are associated with the provided IIN. Then you can verify the card type and check if you can accept this card.

An instance of `OPSession` can be used to check which payment product is associated with an IIN. This is done via the `[session IINDetailsForPartialCreditCardNumber]` function. The result of this check is an instance of `OPIINDetailsResponse`. This class has a property status that indicates the result of the check and a property `paymentProductId` that indicates which payment product is associated with the IIN. The returned `paymentProductId` can be used to provide visual feedback to the customer by showing the appropriate payment product logo.

The `OPIINDetailsResponse` has a status property represented through the `OPIINStatus` enum. The `OPIINStatus` enum values are:
- `OPSupported` indicates that the IIN is associated with a payment product that is supported by our platform.
- `OPUnknown` indicates that the IIN is not recognized.
- `OPNotEnoughDigits` indicates that fewer than six digits have been provided and that the IIN check cannot be performed.
- `OPExistingButNotAllowed` indicates that the provided IIN is recognized, but that the corresponding product is not allowed for the current payment.
```objective-c
[self.session
    IINDetailsForPartialCreditCardNumber:@"123456"
    context:self.context
    success:^(OPIINDetailsResponse *response) {
        // check the status of the associated payment product
        OPIINStatus iinStatus = iinDetailsResponse.status;
    }
    failure:^(NSError *error) {
        // Handle failure of retrieving IIN details
    }
];
```

Some cards are dual branded and could be processed as either a local card _(with a local brand)_ or an international card _(with an international brand)_. In case you are not setup to process these local cards, this API call will not return that card type in its response.

### OPStringFormatter

To help in formatting field values based on masks, the SDK offers the `OPStringFormatter` class. It allows you to format field values and apply and unapply masks on a string.
```objective-c
OPStringFormatter *formatter = [[OPStringFormatter alloc] init];

NSString *mask = @"{{9999}} {{9999}} {{9999}} {{9999}}";
NSString *value = @"1234567890123456";

// apply masked value
NSString *maskedValue = [self.formatter formatStringWithString:value mask:mask]; // "1234 5678 9012 3456"

// remove masked value
NSString *unmaskedValue = [self.formatter unformatString:value withMask:mask]; // "1234567890123456"
```

## Payment Steps

Setting up and completing a payment using the Objective-C SDK involves the following steps:

### 1. Initialize the Objective-C SDK for this payment

This is done using information such as session and customer identifiers, connection URLs and payment context information like currency and total amount.
```objective-c
OPSession *session = [OPSession sessionWithClientSessionId:clientSessionId customerId:customerId baseURL:baseURL assetBaseURL:assetsBaseURL appIdentifier:@"objc Example Application/v1.0.0"];

OPPaymentAmountOfMoney *amountOfMoney = [[OPPaymentAmountOfMoney alloc] initWithTotalAmount:1298 currencyCode:@"EUR"];

OPPaymentContext *paymentContext = [[OPPaymentContext alloc] initWithAmountOfMoney:1298 isRecurring:NO countryCode:@"EUR"];
```

> A successful response from Create Session can be used directly as input for the Session constructor.
- `clientSessionId` / `customerId` properties are used to authentication purposes. These can be obtained your server, using one of our available Server SDKs.
- The `baseURL` and `assetBaseURL` are the URLs the SDK should connect to. The SDK communicates with two types of servers to perform its tasks. One type of server offers the Client API as discussed above. And the other type of server stores the static resources used by the SDK, such as the logos of payment products.
- Payment information (`paymentContext`) is not needed to construct a session, but you will need to provide it when requesting any payment product information. The payment products that the customer can choose from depend on the provided payment information, so the Client SDK needs this information to be able to do its job. The payment information that is needed is:
    - the total amount of the payment, defined as property `amountOfMoney.totalAmount`
    - the currency that should be used, defined as property `amountOfMoney.currencyCodeString`
    - the country of the person that is performing the payment, defined as property `countryCodeString`
    - whether the payment is a single payment or a recurring payment

### 2. Retrieve the payment items

Retrieve the payment products and accounts on file that can be used for this payment. Your application can use this data to create the payment product selection screen.
```objective-c
[self.session
    paymentItemsForContext:paymentContext
    groupPaymentProducts:NO
    success:^(OPPaymentItems *paymentItems) {
        // Display the contents of paymentItems & accountsOnFile to your customer
    }
    failure:^(NSError *error) {
        // Inform the customer that something went wrong while retrieving the available Payment Products
    }
];
```

For some payment products, customers can indicate that they want the Online Payments platform to store part of the data they entered while using such a payment product. For example, it is possible to store the card holder name and the card number for most credit card payment products. The stored data is referred to as an `OPAccountOnFile` or token. `OPAccountOnFile` IDs available for the current payment must be provided in the request body of the Server API's Create Client Session call. When the customer wants to use the same payment product for another payment, it is possible to select one of the stored accounts on file for this payment. In this case, the customer does not have to enter the information that is already stored in the `OPAccountOnFile`. The list of available payment products that the SDK receives from the Client API also contains the accounts on file for each payment product. Your application can present this list of payment products and accounts on file to the customer.

If the customer wishes to use an existing `OPAccountOnFile` for a payment, the selected `OPAccountOnFile` should be added to the `OPPaymentRequest`.

### 3. Retrieve payment product details

Retrieve all the details about the payment product - including it's fields - that the customer needs to provide based on the selected payment product or account on file. Your app can use this information to create the payment product details screen.
```objective-c
[self.session
    paymentProductWithId:@"1"
    context:paymentContext 
    success:^(OPPaymentProduct *paymentProduct) {
        // Display the fields to your customer
    }
    failure:^(NSError *error) {
        // Handle failure of retrieving a Payment Product by id
    }
];
```

Once the customer has selected a payment product or stored account on file, the SDK can request which information needs to be provided by the customer in order to perform a payment. When a single product is retrieved, the SDK provides a list of all the fields that should be rendered, including display hints and validation rules. If the customer selected an account on file, information that is already in this account on file can be prefilled in the input fields, instead of requesting it from the customer. The data that can be stored and prefilled on behalf of the customer is of course in line with applicable regulations. For instance, for a credit card transansaction the customer is still expected to input the CVC. The details entered by the customer are stored in a `OPPaymentRequest`. Again, the example app can be used as the starting point to create your screen. If there is no additional information that needs to be entered, this screen can be skipped.

### 4. Encrypt payment information

Encrypt all the provided payment information in the `OPPaymentRequest` using `[session preparePaymentRequest]`. This function will return a `OPPreparedPaymentRequest` which contains the encrypted payment request fields and encoded client meta info. The encrypted fields result is in a format that can be processed by the Server API. The only thing you need to provide to the SDK are the values the customer provided in your screens. Once you have retrieved the `OPPreparedPaymentRequest ` from the `[session preparePaymentRequest]` function, your application should send it to your server, which in turn should forward it to the Server API.
```objective-c
[self.session
    preparePaymentRequest:paymentRequest
    success:^(OPPreparedPaymentRequest *preparedPaymentRequest) {
        // Pass the encrypted payment request to your server which should then forward it to the Server API
    }
    failure:^(NSError *error) {
        // Handle failure of encrypting a Payment Request
    }
];
```

All the heavy lifting, such as requesting a public key from the Client API, performing the encryption and BASE-64 encoding the result into one string, is done for you by the SDK. You only need to make sure that the `OPPaymentRequest` object contains all the information entered by the user.

From your server, make a create payment request, providing the encrypted data in the `encryptedCustomerInput` field.

### 5. Response from the Server API call
It is up to you and your application to show the customer the correct screens based on the response of the Server API call. In some cases, the payment has not finished yet since the customer must be redirected to a third party (such as a bank or PayPayl) to authorise the payment. See the Server API documentation on what kinds of responses the Server API can return. The Client API has no part in the remainder of the payment.
