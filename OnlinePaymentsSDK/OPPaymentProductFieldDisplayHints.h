//
// Do not remove or alter the notices in this preamble.
// This software code is created for Online Payments on 17/07/2020
// Copyright © 2020 Global Collect Services. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "OPPreferredInputType.h"
#import "OPFormElement.h"
#import "OPTooltip.h"

@interface OPPaymentProductFieldDisplayHints : NSObject

@property (nonatomic) BOOL alwaysShow;
@property (nonatomic) NSInteger displayOrder;
@property (strong, nonatomic) OPFormElement *formElement;
@property (strong, nonatomic) NSString *mask;
@property (nonatomic) BOOL obfuscate;
@property (nonatomic) OPPreferredInputType preferredInputType;
@property (strong, nonatomic) OPTooltip *tooltip;
@property (strong, nonatomic) NSString *label;
@property (strong, nonatomic) NSString *placeholderLabel;
@property (strong, nonatomic) NSURL *link;

- (instancetype)init DEPRECATED_ATTRIBUTE __deprecated_msg("This initialiser is meant for internal SDK usage and should not be used. In the future this contract may change without warning.");

@end
