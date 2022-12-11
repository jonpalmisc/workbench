#import "LIDMainViewController.h"

@interface LIDMainViewController ()

@property (strong) NSTextField* emailField;
@property (strong) NSTextField* passwordField;
@property (strong) NSButton* loginButton;

@end

@implementation LIDMainViewController

- (void)loadView
{
    self.view = [[NSView alloc] init];
    self.title = @"Login Demo";

    _emailField = [[NSTextField alloc] init];
    NSTextField* emailLabel = [NSTextField labelWithString:@"Email:"];
    emailLabel.alignment = NSTextAlignmentRight;

    _passwordField = [[NSSecureTextField alloc] init];
    NSTextField* passwordLabel = [NSTextField labelWithString:@"Password:"];
    passwordLabel.alignment = NSTextAlignmentRight;

    _loginButton = [NSButton buttonWithTitle:@"Login" target:self action:nil];
    _loginButton.translatesAutoresizingMaskIntoConstraints = false;
    _loginButton.keyEquivalent = @"\r";

    NSStackView* emailStackView = [NSStackView stackViewWithViews:@[ emailLabel, _emailField ]];
    emailStackView.alignment = NSLayoutAttributeFirstBaseline;
    NSStackView* passwordStackView = [NSStackView stackViewWithViews:@[ passwordLabel, _passwordField ]];
    passwordStackView.alignment = NSLayoutAttributeFirstBaseline;

    NSStackView* mainStackView = [NSStackView stackViewWithViews:@[
        emailStackView, passwordStackView, _loginButton
    ]];
    mainStackView.orientation = NSUserInterfaceLayoutOrientationVertical;

    [self.view addSubview:mainStackView];

    [NSLayoutConstraint activateConstraints:@[
        [_emailField.widthAnchor constraintGreaterThanOrEqualToConstant:256.0],
        [_passwordField.widthAnchor constraintGreaterThanOrEqualToConstant:256.0],

        // Effectively right-align all field labels.
        [emailLabel.widthAnchor constraintEqualToAnchor:passwordLabel.widthAnchor],

        // Right-align the login button.
        [_loginButton.topAnchor constraintEqualToSystemSpacingBelowAnchor:_passwordField.bottomAnchor multiplier:2],
        [_loginButton.trailingAnchor constraintEqualToAnchor:mainStackView.trailingAnchor],
        [_loginButton.widthAnchor constraintEqualToConstant:72.0],

        [mainStackView.leadingAnchor constraintEqualToSystemSpacingAfterAnchor:self.view.leadingAnchor multiplier:1],
        [mainStackView.topAnchor constraintEqualToSystemSpacingBelowAnchor:self.view.topAnchor multiplier:1],
        [self.view.trailingAnchor constraintEqualToSystemSpacingAfterAnchor:mainStackView.trailingAnchor multiplier:1],
        [self.view.bottomAnchor constraintEqualToSystemSpacingBelowAnchor:mainStackView.bottomAnchor multiplier:1]
    ]];
}

@end
