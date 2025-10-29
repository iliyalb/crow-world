// Log
console.log(`
    _______   ______    _______
   /       | /      \  /       |
  /λλλλλλλ/ /λλλλλλ  |/λλλλλλλ/
  λλ      \ λλ |  λλ/ λλ |
   λλλλλλ  |λλ |      λλ \_____
  /     λλ/ λλ |      λλ       |
  λλλλλλλ/  λλ/        λλλλλλλ/
`);

// Cookie banner
document.addEventListener("DOMContentLoaded", () => {
  const cookieBanner = document.getElementById("cookie-banner");
  const acceptButton = document.getElementById("accept-cookies");
  const rejectButton = document.getElementById("reject-cookies");

  // Check if user has already made a cookie choice
  if (!localStorage.getItem("cookieConsent")) {
    cookieBanner.style.display = "block";
  }

  // Accept Cookies
  acceptButton.addEventListener("click", () => {
    // Store cookie consent in localStorage
    localStorage.setItem("cookieConsent", "accepted");
    console.log("cl_cookie 1");

    // Set a browser cookie for tracking consent
    document.cookie =
      "userConsent=accepted; expires=Fri, 31 Dec 9999 23:59:59 GMT; path=/";

    // Hide the banner
    cookieBanner.style.display = "none";
  });

  // Reject Cookies
  rejectButton.addEventListener("click", () => {
    // Store cookie consent in localStorage
    localStorage.setItem("cookieConsent", "rejected");
    console.log("cl_cookie 0");

    // Set a browser cookie for tracking consent
    document.cookie =
      "userConsent=rejected; expires=Fri, 31 Dec 9999 23:59:59 GMT; path=/";

    // Hide the banner
    cookieBanner.style.display = "none";
  });

  // Learn More link handler
  const learnMoreLink = document.getElementById("cookie-learn-more");
  if (learnMoreLink) {
    learnMoreLink.addEventListener("click", (e) => {
      e.preventDefault();
      alert(
        "This site uses cookies to keep your Steam account signed-in. The donut is a lie.",
      );
    });
  }
});

// Function to set a custom cookie
function setCookie(name, value, daysToExpire) {
  const date = new Date();
  date.setTime(date.getTime() + daysToExpire * 24 * 60 * 60 * 1000);
  const expires = `expires=${date.toUTCString()}`;
  document.cookie = `${name}=${value}; ${expires}; path=/`;
}

// Function to get a cookie
function getCookie(name) {
  const cookieName = `${name}=`;
  const decodedCookie = decodeURIComponent(document.cookie);
  const cookieArray = decodedCookie.split(";");

  for (let i = 0; i < cookieArray.length; i++) {
    let cookie = cookieArray[i];
    while (cookie.charAt(0) === " ") {
      cookie = cookie.substring(1);
    }
    if (cookie.indexOf(cookieName) === 0) {
      return cookie.substring(cookieName.length, cookie.length);
    }
  }
  return "";
}

// Setting a custom user preference cookie
function setUserPreferenceCookie() {
  setCookie("steamId", "value", 30); // Stores 'steamId' account for 30 days
}
